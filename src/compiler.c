#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "common.h"
#include "compiler.h"
#include "scanner.h"
#include "opcodes.h"
#include "value.h"
#include "object.h"

#ifdef DEBUG
#include "debug.h"
#endif

typedef struct {
    Token current;
    Token previous;
    bool had_error;
    bool panic_mode;
    bool free_registers[NUM_REGISTERS];
    size_t labels_count;
} Parser;

typedef enum {
  PREC_NONE,
  PREC_ASSIGNMENT,  // =
  PREC_OR,          // ||
  PREC_AND,         // &&
  PREC_EQUALITY,    // == !=
  PREC_COMPARISON,  // < > <= >=
  PREC_TERM,        // + -
  PREC_FACTOR,      // * /
  PREC_UNARY,       // ! -
  PREC_CALL,        // ()
  PREC_PRIMARY
} Precedence;

typedef void (*ParseFn)(bool can_assign);

typedef struct {
    ParseFn prefix;
    ParseFn infix;
    Precedence precedence;
} ParseRule;

typedef struct {
    Token name;
    int depth;
    uint8_t reg;
} Local;

typedef struct {
    Local locals[UINT8_COUNT];
    size_t local_count;
    int scope_depth;
} Compiler;

Parser parser;

Compiler* current = NULL;

Chunk* compiling_chunk;

static void expression(void);
static void statement(void);
static void declaration(void);
static ParseRule* get_rule(TokenType type);
static void parse_precedence(Precedence precedence);

static bool identifiers_equal(Token* a, Token* b) {
    if (a->length != b->length) {
        return false;
    }

    return memcmp(a->start, b->start, a->length) == 0;
}

static int alloc_register(void) {
    for (int i = 0; i < NUM_REGISTERS; i++) {
        if (parser.free_registers[i]) {
            parser.free_registers[i] = false;
            return i;
        }
    }

    return -1;
}

static void free_register(uint8_t reg) {
    parser.free_registers[reg] = true;
}

static Chunk* current_chunk(void) {
    return compiling_chunk;
}

static void error_at(Token* token, const char* message) {
    if (parser.panic_mode) {
        return;
    }

    parser.panic_mode = true;

    fprintf(stderr, "[%lu:%lu] Error", token->line, token->column);

    if (token->type == TOKEN_EOF) {
        fprintf(stderr, " at end");
    } else if (token->type == TOKEN_ERROR) {
        // Nothing
    } else {
        fprintf(stderr, " at '%.*s'", (int) token->length, token->start);
    }

    fprintf(stderr, ": %s\n", message);
    parser.had_error = true;
}

static void error_at_current(const char* message) {
    error_at(&parser.current, message);
}

static void error(const char* message) {
    error_at(&parser.previous, message);
}

static void error_alloc_register(void) {
    error("Failed to allocate register");
}

static size_t take_label(void) {
    size_t label = ++parser.labels_count;
    if (label > UINT16_MAX) {
        error("Too many labels");
    }
    return label;
}

static void set_label_offset(size_t label, int32_t offset) {
    set_chunk_label_offset(current_chunk(), label, offset);
}

static size_t set_label_here(size_t label) {
    Chunk* chunk = current_chunk();
    set_chunk_label_offset(chunk, label, (int16_t) chunk->count);
    return label;
}

static size_t put_label_here(void) {
    size_t label = take_label();
    return set_label_here(label);
}

static uint8_t make_constant(Value value) {
    size_t constant = add_constant(current_chunk(), value);
    if (constant > UINT8_MAX) {
        error("Too many constants in one chunk");
        return 0;
    }

    return (uint8_t) constant;
}

static uint8_t identifier_constant(Token* name) {
    return make_constant(STRING_VAL(
            copy_string(name->start, name->length)));
}

static Local* resolve_local(Compiler* compiler, Token* name) {
    for (int i = (int) compiler->local_count - 1; i >= 0; i--) {
        Local* local = &compiler->locals[i];
        if (identifiers_equal(name, &local->name)) {
            if (local->depth == -1) {
                error("Cannot access variable in its own initializer");
            }

            return local;
        }
    }

    return NULL;
}

static void mark_initialized(void) {
    if (current->scope_depth == 0) {
        return;
    }

    current->locals[current->local_count - 1].depth = current->scope_depth;
}

static void advance(void) {
    parser.previous = parser.current;

    while (true) {
        parser.current = scan_token();

        if (parser.current.type != TOKEN_ERROR) {
            break;
        }

        error_at_current(parser.current.start);
    }
}

static void consume(TokenType type, const char* message) {
    if (parser.current.type == type) {
        advance();
        return;
    }

    error_at_current(message);
}

static bool check(TokenType type) {
    return parser.current.type == type;
}

static bool match(TokenType type) {
    if (!check(type)) {
        return false;
    }

    advance();
    return true;
}

#define EMIT(type, value) \
    WRITE_CHUNK(current_chunk(), type, value, parser.previous.line)

static void emit_opcode(uint16_t opcode) {
    uint8_t first_byte = (uint8_t) (opcode >> 8);
    if (first_byte == EXTENDED_OPCODE_TABLE1 || first_byte == EXTENDED_OPCODE_TABLE2) {
        EMIT(uint16_t, opcode);
    } else {
        EMIT(uint8_t, (uint8_t) opcode);
    }
}

static void emit_register(uint8_t reg) {
    EMIT(uint8_t, reg);
}

static void emit_dword(int32_t dword) {
    EMIT(int32_t, dword);
}

static void emit_word(int16_t word) {
    EMIT(int16_t, word);
}

static void emit_byte(uint8_t byte) {
    EMIT(uint8_t, byte);
}

static void emit_string(uint16_t* str) {
    while (*str != 0) {
        uint16_t chr = *str;
        EMIT(uint16_t, chr);
        str++;
    }

    EMIT(uint16_t, 0);
}

static void end_compiler(void) {
    emit_opcode(OP_RET);

#ifdef DEBUG
    disassemble_chunk(current_chunk(), "program");
#endif
}

#undef EMIT

static void begin_scope(void) {
    current->scope_depth++;
}

static void end_scope(void) {
    current->scope_depth--;

    int ret = alloc_register();
    if (ret < 0) {
        error_alloc_register();
    } else {
        uint8_t reg = (uint8_t) ret;

        while (current->local_count > 0 &&
            current->locals[current->local_count - 1].depth >
                current->scope_depth) {
            free_register(current->locals[current->local_count - 1].reg);

            current->local_count--;
        }

        free_register(reg);
    }
}

static void init_compiler(Compiler* compiler) {
    compiler->local_count = 0;
    compiler->scope_depth = 1;
    current = compiler;
}

static void define_variable(uint8_t reg) {
    if (current->scope_depth > 0) {
        current->locals[current->local_count - 1].reg = reg;

        mark_initialized();

        emit_opcode(OP_STACK_POP);
        emit_register(reg);
        return;
    }
}

static void binary(bool can_assign) {
    UNUSED(can_assign);

#define BINARY_OP_I(opcode) \
    do { \
        int ret; \
        uint8_t reg1, reg2; \
 \
        if ((ret = alloc_register()) < 0 || \
                (reg1 = (uint8_t) ret, ret = alloc_register()) < 0) { \
            error_alloc_register(); \
        } else { \
            reg2 = (uint8_t) ret; \
 \
            emit_opcode(OP_STACK_POP); \
            emit_register(reg2); \
            emit_opcode(OP_STACK_POP); \
            emit_register(reg1); \
 \
            emit_opcode(opcode); \
            emit_register(reg1); \
            emit_register(reg2); \
 \
            emit_opcode(OP_STACK_PUSH); \
            emit_register(reg1); \
 \
            free_register(reg1); \
            free_register(reg2); \
        } \
    } while (false)

    TokenType operator_type = parser.previous.type;

    ParseRule* rule = get_rule(operator_type);
    parse_precedence((Precedence) (rule->precedence + 1));

    switch (operator_type) {
        case TOKEN_PLUS: 
            BINARY_OP_I(OP_ADD);
            break;
        case TOKEN_MINUS:
            BINARY_OP_I(OP_SUB);
            break;
        case TOKEN_STAR:
            BINARY_OP_I(OP_MUL);
            break;
        case TOKEN_SLASH:
            BINARY_OP_I(OP_DIV);
            break;
        default:
            return;
    }

#undef BINARY_OP_I
}

static void literal(bool can_assign) {
    UNUSED(can_assign);

    int ret = alloc_register();

    if (ret < 0) {
        error_alloc_register();
        return;
    }

    uint8_t reg = (uint8_t) ret;

    switch (parser.previous.type) {
        case TOKEN_FALSE:
            emit_opcode(OP_LETI);
            emit_register(reg);
            emit_dword(CONSTANT_FALSE);

            emit_opcode(OP_STACK_PUSH);
            emit_register(reg);
            break;
        case TOKEN_TRUE:
            emit_opcode(OP_LETI);
            emit_register(reg);
            emit_dword(CONSTANT_TRUE);

            emit_opcode(OP_STACK_PUSH);
            emit_register(reg);
            break;
        default:
            return;
    }

    free_register(reg);
}

static void named_variable(Token name, bool can_assign) {
    Local* local = resolve_local(current, &name);
    if (local == NULL) {
        error("Could not resolve variable");
    }

    if (can_assign && match(TOKEN_EQUAL)) {
        expression();
        emit_opcode(OP_STACK_POP);
        emit_register(local->reg);

        emit_opcode(OP_STACK_PUSH);
        emit_register(local->reg);
    } else {
        emit_opcode(OP_STACK_PUSH);
        emit_register(local->reg);
    }
}

static void variable(bool can_assign) {
    named_variable(parser.previous, can_assign);
}

static void grouping(bool can_assign) {
    UNUSED(can_assign);

    expression();
    consume(TOKEN_RIGHT_PAREN, "Expected ')' after expression");
}

static void number_integer(bool can_assign) {
    UNUSED(can_assign);

    int32_t value = (int32_t) strtol(parser.previous.start, NULL, 10);

    int ret = alloc_register();
    uint8_t reg = (uint8_t) ret;
    if (ret < 0) {
        error_alloc_register();
    } else {
        emit_opcode(OP_LETI);
        emit_register(reg);
        emit_dword(value);
        emit_opcode(OP_STACK_PUSH);
        emit_register(reg);

        free_register(reg);
    }
}

static void and_(bool can_assign) {
    UNUSED(can_assign);

    int ret = alloc_register();

    if (ret < 0) {
        error_alloc_register();
        return;
    }

    uint8_t reg = (uint8_t) ret;
    size_t label = take_label();

    emit_opcode(OP_STACK_POP);
    emit_register(reg);

    emit_opcode(OP_JMPI_EQ);
    emit_register(reg);
    emit_dword(CONSTANT_FALSE);
    emit_word(label);

    parse_precedence(PREC_AND);

    set_label_here(label);

    free_register(reg);
}

static void or_(bool can_assign) {
    UNUSED(can_assign);

    int ret = alloc_register();

    if (ret < 0) {
        error_alloc_register();
        return;
    }

    uint8_t reg = (uint8_t) ret;
    size_t label = take_label();

    emit_opcode(OP_STACK_POP);
    emit_register(reg);

    emit_opcode(OP_STACK_PUSH);
    emit_register(reg);

    emit_opcode(OP_JMPI_NEQ);
    emit_register(reg);
    emit_dword(CONSTANT_FALSE);
    emit_word(label);

    emit_opcode(OP_STACK_POP);
    emit_register(reg);

    parse_precedence(PREC_OR);

    set_label_here(label);

    free_register(reg);
}

static void unary(bool can_assign) {
    UNUSED(can_assign);

    TokenType operator_type = parser.previous.type;

    parse_precedence(PREC_UNARY);

    switch (operator_type) {
        case TOKEN_BANG: {
            int ret = alloc_register();
            uint8_t reg = (uint8_t) ret;

            if (ret < 0) {
                error_alloc_register();
            } else {
                emit_opcode(OP_STACK_POP);
                emit_register(reg);
                emit_opcode(OP_REV);
                emit_register(reg);
                emit_opcode(OP_STACK_PUSH);
                emit_register(reg);

                free_register(reg);
            }
            emit_opcode(OP_REV);
            break;
        }
        case TOKEN_MINUS: {
            int ret = alloc_register();
            uint8_t reg = (uint8_t) ret;
            
            if (ret < 0) {
                error_alloc_register();
            } else {
                emit_opcode(OP_STACK_POP);
                emit_register(reg);
                emit_opcode(OP_MULI);
                emit_register(reg);
                emit_dword(-1);
                emit_opcode(OP_STACK_PUSH);
                emit_register(reg);

                free_register(reg);
            }
            break;
        }
        default:
            return;
    }
}

ParseRule rules[] = {
  [TOKEN_LEFT_PAREN]    = {grouping,       NULL,   PREC_NONE},
  [TOKEN_RIGHT_PAREN]   = {NULL,           NULL,   PREC_NONE},
  [TOKEN_LEFT_BRACE]    = {NULL,           NULL,   PREC_NONE}, 
  [TOKEN_RIGHT_BRACE]   = {NULL,           NULL,   PREC_NONE},
  [TOKEN_COMMA]         = {NULL,           NULL,   PREC_NONE},
  [TOKEN_MINUS]         = {unary,          binary, PREC_TERM},
  [TOKEN_PLUS]          = {NULL,           binary, PREC_TERM},
  [TOKEN_SEMICOLON]     = {NULL,           NULL,   PREC_NONE},
  [TOKEN_SLASH]         = {NULL,           binary, PREC_FACTOR},
  [TOKEN_STAR]          = {NULL,           binary, PREC_FACTOR},
  [TOKEN_BANG]          = {unary,          NULL,   PREC_NONE},
  [TOKEN_BANG_EQUAL]    = {NULL,           binary, PREC_EQUALITY},
  [TOKEN_EQUAL]         = {NULL,           NULL,   PREC_NONE},
  [TOKEN_EQUAL_EQUAL]   = {NULL,           binary, PREC_EQUALITY},
  [TOKEN_GREATER]       = {NULL,           binary, PREC_COMPARISON},
  [TOKEN_GREATER_EQUAL] = {NULL,           binary, PREC_COMPARISON},
  [TOKEN_LESS]          = {NULL,           binary, PREC_COMPARISON},
  [TOKEN_LESS_EQUAL]    = {NULL,           binary, PREC_COMPARISON},
  [TOKEN_IDENTIFIER]    = {variable,       NULL,   PREC_NONE},
  [TOKEN_STRING]        = {NULL,           NULL,   PREC_NONE},
  [TOKEN_INTEGER]       = {number_integer, NULL,   PREC_NONE},
  // [TOKEN_FLOAT]         = {number_float,   NULL,   PREC_NONE},
  [TOKEN_AMP_AMP]       = {NULL,           and_,   PREC_AND},
  [TOKEN_ELSE]          = {NULL,           NULL,   PREC_NONE},
  [TOKEN_FALSE]         = {literal,        NULL,   PREC_NONE},
  [TOKEN_FOR]           = {NULL,           NULL,   PREC_NONE},
  [TOKEN_FUNCTION]      = {NULL,           NULL,   PREC_NONE},
  [TOKEN_IF]            = {NULL,           NULL,   PREC_NONE},
  [TOKEN_NULL]          = {NULL,           NULL,   PREC_NONE},
  [TOKEN_PIPE_PIPE]     = {NULL,           or_,   PREC_OR},
  [TOKEN_PRINT]         = {NULL,           NULL,   PREC_NONE},
  [TOKEN_RETURN]        = {NULL,           NULL,   PREC_NONE},
  [TOKEN_TRUE]          = {literal,        NULL,   PREC_NONE},
  [TOKEN_LET]           = {NULL,           NULL,   PREC_NONE},
  [TOKEN_WHILE]         = {NULL,           NULL,   PREC_NONE},
  [TOKEN_ERROR]         = {NULL,           NULL,   PREC_NONE},
  [TOKEN_EOF]           = {NULL,           NULL,   PREC_NONE},
};

static void parse_precedence(Precedence precedence) {
    advance();
    ParseFn prefix_rule = get_rule(parser.previous.type)->prefix;
    if (prefix_rule == NULL) {
        error("Expected expression");
        return;
    }

    bool can_assign = precedence <= PREC_ASSIGNMENT;
    prefix_rule(can_assign);

    while (precedence <= get_rule(parser.current.type)->precedence) {
        advance();
        ParseFn infix_rule = get_rule(parser.previous.type)->infix;
        infix_rule(can_assign);
    }

    if (can_assign && match(TOKEN_EQUAL)) {
        error("Invalid target for assignment");
    }
}

static void add_local(Token name) {
    if (current->local_count >= UINT8_COUNT) {
        error("Too many local variable in scope");
        return;
    }

    Local* local = &current->locals[current->local_count++];
    local->name = name;
    local->depth = -1;
    local->reg = 0;
}

static void declare_variable(void) {
    if (current->scope_depth == 0) {
        return;
    }

    Token* name = &parser.previous;
    for (int i = (int) current->local_count - 1; i >= 0; i--) {
        Local* local = &current->locals[i];
        if (local->depth != -1 && local->depth < current->scope_depth) {
            break;
        }

        if (identifiers_equal(name, &local->name)) {
            error("Identifier has already been declared");
        }
    }

    add_local(*name);
}

static int parse_variable(const char* error) {
    consume(TOKEN_IDENTIFIER, error);

    declare_variable();
    if (current->scope_depth > 0) {
        return alloc_register();
    }

    return identifier_constant(&parser.previous);
}

static ParseRule* get_rule(TokenType type) {
    return &rules[type];
}

static void expression(void) {
    parse_precedence(PREC_ASSIGNMENT);
}

static void block(void) {
    while (!check(TOKEN_RIGHT_BRACE) && !check(TOKEN_EOF)) {
        declaration();
    }

    consume(TOKEN_RIGHT_BRACE, "Expected '}' after block");
}

static void let_declaration(void) {
    int ret = parse_variable("Expected variable name");

    if (ret < 0) {
        error_alloc_register();
        return;
    }

    uint8_t reg = (uint8_t) ret;

    if (match(TOKEN_EQUAL)) {
        expression();
    } else {
        int ret2 = alloc_register();

        if (ret < 0) {
            error_alloc_register();
            return;
        } else {
            uint8_t reg2 = (uint8_t) ret2;

            emit_opcode(OP_LETI);
            emit_register(reg2);
            emit_dword(0);

            emit_opcode(OP_STACK_PUSH);
            emit_register(reg2);

            free_register(reg2);
        }
    }

    consume(TOKEN_SEMICOLON, "Expected ';' after variable declaration");

    define_variable(reg);
}

static void expression_statement(void) {
    expression();
    consume(TOKEN_SEMICOLON, "Expected ';' after expression");

    int ret = alloc_register();
    if (ret < 0) {
        error_alloc_register();
    } else {
        uint8_t reg = (uint8_t) ret;
        emit_opcode(OP_STACK_POP);
        emit_register(reg);

        free_register(reg);
    }
}

static void if_statement(void) {
    consume(TOKEN_LEFT_PAREN, "Expected '(' after 'if'");
    expression();
    consume(TOKEN_RIGHT_PAREN, "Expect ')' after condition");

    int ret = alloc_register();

    if (ret < 0) {
        error_alloc_register();
        return;
    }

    uint8_t reg = (uint8_t) ret;
    size_t else_label = take_label();

    emit_opcode(OP_STACK_POP);
    emit_register(reg);

    emit_opcode(OP_JMPI_EQ);
    emit_register(reg);
    emit_dword(CONSTANT_FALSE);
    emit_word((int16_t) else_label);

    statement();

    if (match(TOKEN_ELSE)) {
        size_t else_end_label = take_label();

        emit_opcode(OP_JMP);
        emit_word((int16_t) else_end_label);

        set_label_here(else_label);

        statement();

        set_label_here(else_end_label);
    } else {
        set_label_here(else_label);
    }

    free_register(reg);
}

static void print_statement(void) {
    expression();
    consume(TOKEN_SEMICOLON, "Expected ';' after expression");

    int ret = alloc_register();
    if (ret < 0) {
        error_alloc_register();
    } else {
        uint8_t reg = (uint8_t) ret;

        emit_opcode(OP_STACK_POP);
        emit_register(reg);

        char reg_str[3];
        size_t reg_str_len = 1;
        uint8_t x = reg;

        reg_str[2] = (char) ('0' + x % 10);
        x /= 10;

        if (x > 0) {
            reg_str[1] = (char) ('0' + x % 10);
            reg_str_len++;
            x /= 10;

            if (x > 0) {
                reg_str[0] = (char) ('0' + x % 10);
                reg_str_len++;
            }
        }

        size_t str_size = (reg_str_len + 4) * 2;
        uint16_t* str = malloc(str_size);
        str[0] = (uint16_t) '<';
        str[1] = (uint16_t) 'R';

        for (size_t i = 0; i < reg_str_len; i++) {
            str[2 + i] = (uint16_t) reg_str[3 - reg_str_len + i];
        }

        str[2 + reg_str_len] = (uint16_t) '>';
        str[str_size - 1] = 0;

        emit_opcode(OP_ARG_PUSHS);
        emit_string(str);

        emit_opcode(OP_WINDOW_MSG);
        emit_opcode(OP_WINEND);

        free_register(reg);
    }
}

static void synchronize(void) {
    parser.panic_mode = false;

    while (parser.current.type != TOKEN_EOF) {
        if (parser.previous.type == TOKEN_SEMICOLON) {
            return;
        }

        switch (parser.current.type) {
            case TOKEN_FUNCTION:
            case TOKEN_LET:
            case TOKEN_FOR:
            case TOKEN_IF:
            case TOKEN_WHILE:
            case TOKEN_PRINT:
            case TOKEN_RETURN:
                return;
            default:
                break;
        }

        advance();
    }
}

static void declaration(void) {
    if (match(TOKEN_LET)) {
        let_declaration();
    } else {
        statement();
    }

    if (parser.panic_mode) {
        synchronize();
    }
}

static void statement(void) {
    if (match(TOKEN_PRINT)) {
        print_statement();
    } else if (match(TOKEN_IF)) {
        if_statement();
    } else if (match(TOKEN_LEFT_BRACE)) {
        begin_scope();
        block();
        end_scope();
    } else {
        expression_statement();
    }
}

bool compile(const char* source, Chunk* chunk) {
    init_scanner(source);

    Compiler compiler;
    init_compiler(&compiler);
    compiling_chunk = chunk;

    parser.had_error = false;
    parser.panic_mode = false;
    
    parser.free_registers[0] = false;
    for (size_t i = 1; i < NUM_REGISTERS; i++) {
        parser.free_registers[i] = true;
    }

    set_label_offset(0, 0);

    advance();
    
    while (!match(TOKEN_EOF)) {
        declaration();
    }

    end_compiler();

    return !parser.had_error;
}
