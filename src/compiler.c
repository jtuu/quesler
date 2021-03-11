#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "common.h"
#include "compiler.h"
#include "scanner.h"
#include "opcodes.h"
#include "value.h"
#include "object.h"
#include "memory.h"

#ifdef DEBUG
#include "debug.h"
#endif

typedef struct {
    Token name;
    size_t label;
    size_t arity;
} Function;

typedef struct {
    Token current;
    Token previous;
    Token previous2;
    bool had_error;
    bool panic_mode;
    bool free_registers[NUM_REGISTERS];
    size_t labels_count;
    bool entry_function_exists;
    Function** functions;
    size_t functions_count;
    size_t functions_capacity;
    size_t current_function_label;
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
    bool defined;
    bool allocated;
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

size_t entry_point_label = 0;
const char* entry_function_name = "main";
size_t entry_function_name_len = 4;

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

static void mark_initialized(Local* local) {
    if (current->scope_depth == 0) {
        return;
    }

    local->depth = current->scope_depth;
}

static void advance(void) {
    parser.previous2 = parser.previous;
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
#ifdef DEBUG
    disassemble_chunk(current_chunk(), "program");
#endif
}

#undef EMIT

static void emit_leti(uint8_t reg, int32_t val) {
    emit_opcode(OP_LETI);
    emit_register(reg);
    emit_dword(val);
}

static void emit_stack_pop(uint8_t reg) {
    emit_opcode(OP_STACK_POP);
    emit_register(reg);
}

static void emit_stack_push(uint8_t reg) {
    emit_opcode(OP_STACK_PUSH);
    emit_register(reg);
}

static void emit_stack_peek(uint8_t reg) {
    emit_stack_pop(reg);
    emit_stack_push(reg);
}

static void emit_stack_swap(void) {
    int ret = alloc_register();

    if (ret < 0) {
        error_alloc_register();
        return;
    }

    uint8_t reg1 = (uint8_t) ret;

    ret = alloc_register();

    if (ret < 0) {
        free_register(reg1);
        error_alloc_register();
        return;
    }

    uint8_t reg2 = (uint8_t) ret;

    emit_stack_pop(reg1);
    emit_stack_pop(reg2);
    emit_stack_push(reg1);
    emit_stack_push(reg2);

    free_register(reg1);
    free_register(reg2);
}

static void emit_stack_push_value(int32_t val) {
    int ret = alloc_register();

    if (ret < 0) {
        error_alloc_register();
        return;
    }

    uint8_t reg = (uint8_t) ret;

    emit_leti(reg, val);
    emit_stack_push(reg);

    free_register(reg);
}

static void emit_call(size_t label) {
    emit_opcode(OP_CALL);
    emit_word((int16_t) label);
}

static void emit_jump(size_t label) {
    emit_opcode(OP_JMP);
    emit_word((int16_t) label);
}

static void emit_conditional_jump_rr(uint16_t opcode, uint8_t reg1, uint8_t reg2, size_t label) {
    emit_opcode(opcode);
    emit_register(reg1);
    emit_register(reg2);
    emit_word((int16_t) label);
}

static void emit_conditional_jump_ri(uint16_t opcode, uint8_t reg, int32_t val, size_t label) {
    emit_opcode(opcode);
    emit_register(reg);
    emit_dword(val);
    emit_word((int16_t) label);
}

static void emit_runtime(void) {
    // Reserved registers
    parser.free_registers[0] = false;
}

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

static void define_function(Function* func) {
    if (parser.functions_capacity < parser.functions_count + 1) {
        size_t old_capacity = parser.functions_capacity;
        parser.functions_capacity = GROW_CAPACITY(old_capacity);
        parser.functions = GROW_ARRAY(Function*, parser.functions,
            old_capacity, parser.functions_capacity);
    }

    parser.functions[parser.functions_count] = func;
    parser.functions_count++;
}

static void define_variable(uint8_t reg, Local* local) {
    if (current->scope_depth > 0) {
        local->allocated = true;
        local->reg = reg;
        mark_initialized(local);
        emit_stack_pop(reg);
        return;
    }
}

static size_t argument_list(void) {
    size_t arg_count = 0;

    if (!check(TOKEN_RIGHT_PAREN)) {
        do {
            expression();
            arg_count++;
        } while (match(TOKEN_COMMA));
    }

    consume(TOKEN_RIGHT_PAREN, "Expected ')' after function call arguments");

    return arg_count;
}

static void binary(bool can_assign) {
    UNUSED(can_assign);

    int ret;
    uint8_t reg1, reg2;

    if ((ret = alloc_register()) < 0) {
        error_alloc_register();
        return;
    }

    reg1 = (uint8_t) ret;

    if ((ret = alloc_register()) < 0) {
        free_register(reg1);
        error_alloc_register();
        return;
    }

    reg2 = (uint8_t) ret;

#define BINARY_OP_I(opcode) \
    do { \
        emit_stack_pop(reg2); \
        emit_stack_pop(reg1); \
 \
        emit_opcode(opcode); \
        emit_register(reg1); \
        emit_register(reg2); \
 \
        emit_stack_push(reg1); \
 \
        free_register(reg1); \
        free_register(reg2); \
    } while (false)

#define COMPARISON_OP(opcode) \
    do { \
        size_t label_true = take_label(); \
        size_t label_continue = take_label(); \
 \
        emit_stack_pop(reg2); \
        emit_stack_pop(reg1); \
 \
        emit_conditional_jump_rr(opcode, reg1, reg2, label_true); \
 \
        emit_stack_push_value(CONSTANT_FALSE); \
        emit_jump(label_continue); \
 \
        set_label_here(label_true); \
        emit_stack_push_value(CONSTANT_TRUE); \
 \
        set_label_here(label_continue); \
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
        case TOKEN_EQUAL_EQUAL:
            COMPARISON_OP(OP_JMP_EQ);
            break;
        case TOKEN_BANG_EQUAL:
            COMPARISON_OP(OP_JMP_NEQ);
            break;
        case TOKEN_GREATER:
            COMPARISON_OP(OP_JMP_GT);
            break;
        case TOKEN_LESS:
            COMPARISON_OP(OP_JMP_LT);
            break;
        case TOKEN_GREATER_EQUAL:
            COMPARISON_OP(OP_JMP_GTE);
            break;
        case TOKEN_LESS_EQUAL:
            COMPARISON_OP(OP_JMP_LTE);
            break;
        default:
            error("Unknown token");
            break;
    }

    free_register(reg1);
    free_register(reg2);

#undef BINARY_OP_I
}

static Function* resolve_function(Token* name) {
    for (size_t i = 0; i < parser.functions_count; i++) {
        if (parser.functions[i]->name.length == name->length) {
            if (!strncmp(parser.functions[i]->name.start, name->start, name->length)) {
                return parser.functions[i];
            }
        }
    }

    return NULL;
}

static void call(bool can_assign) {
    UNUSED(can_assign);

    Function* func = resolve_function(&parser.previous2);
    if (func == NULL) {
        error("Call target is not a function");
        return;
    }

    size_t arg_count = argument_list();

    if (arg_count > func->arity) {
        error("Too many arguments");
        return;
    } else if (arg_count < func->arity) {
        error("Not enough arguments");
        return;
    }

    uint8_t* regs = malloc(arg_count * sizeof(uint8_t));

    for (size_t i = 0; i < arg_count; i++) {
        int ret = alloc_register();

        if (ret < 0) {
            error_alloc_register();

            for (int j = (int) i; j >= 0; j--) {
                free_register(regs[j]);
            }

            break;
        }

        regs[i] = (uint8_t) ret;
        emit_stack_pop(regs[i]);
    }
    
    for (size_t i = 0; i < current->local_count; i++) {
        emit_stack_push(current->locals[i].reg);
    }

    for (int i = (int) arg_count; i >= 0; i--) {
        emit_stack_push(regs[i]);
        free_register(regs[i]);
    }

    free(regs);

    emit_call(func->label);

    for (int i = (int) current->local_count; i >= 0; i--) {
        emit_stack_pop(current->locals[i].reg);
    }
}

static void literal(bool can_assign) {
    UNUSED(can_assign);

    switch (parser.previous.type) {
        case TOKEN_FALSE:
            emit_stack_push_value(CONSTANT_FALSE);
            break;
        case TOKEN_TRUE:
            emit_stack_push_value(CONSTANT_TRUE);
            break;
        default:
            error("Unknown literal token");
            break;
    }
}

static void named_variable(Token name, bool can_assign) {
    Local* local = resolve_local(current, &name);
    if (local == NULL) {
        Function* function = resolve_function(&name);

        if (function == NULL) {
            error("Could not resolve variable");
        }
    } else {
        if (can_assign && match(TOKEN_EQUAL)) {
            // Variable write
            expression();
            emit_stack_peek(local->reg);
        } else if (local->depth != -1) {
            // Variable read
            emit_stack_push(local->reg);
        }
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

    emit_stack_push_value(value);
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

    emit_stack_pop(reg);

    emit_conditional_jump_ri(OP_JMPI_EQ, reg, CONSTANT_FALSE, label);

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

    emit_stack_peek(reg);

    emit_conditional_jump_ri(OP_JMPI_NEQ, reg, CONSTANT_FALSE, label);

    emit_stack_pop(reg);

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
                emit_stack_pop(reg);
                emit_opcode(OP_REV);
                emit_register(reg);
                emit_stack_push(reg);

                free_register(reg);
            }
            break;
        }
        case TOKEN_MINUS: {
            int ret = alloc_register();
            uint8_t reg = (uint8_t) ret;
            
            if (ret < 0) {
                error_alloc_register();
            } else {
                emit_stack_pop(reg);
                emit_opcode(OP_MULI);
                emit_register(reg);
                emit_dword(-1);
                emit_stack_push(reg);

                free_register(reg);
            }
            break;
        }
        default:
            return;
    }
}

ParseRule rules[] = {
  [TOKEN_LEFT_PAREN]    = {grouping,       call,   PREC_CALL},
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

static Local* add_local(Token name) {
    if (current->local_count >= UINT8_COUNT) {
        error("Too many local variable in scope");
        return NULL;
    }

    Local* local = &current->locals[current->local_count++];
    local->name = name;
    local->depth = -1;
    local->defined = false;
    local->allocated = false;
    local->reg = 0;

    return local;
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

    return -1;
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

static Function* function(void) {
    int ret = alloc_register();

    if (ret < 0) {
        error_alloc_register();
        return NULL;
    }

    uint8_t reg = (uint8_t) ret;

    begin_scope();

    Function* func = malloc(sizeof(Function));
    func->name = parser.previous;
    func->arity = 0;

    define_function(func);

    bool is_entry_function = false;

    if (func->name.length == entry_function_name_len && 
            !strncmp(func->name.start, entry_function_name, entry_function_name_len)) {
        parser.entry_function_exists = true;
        func->label = entry_point_label;
        set_label_here(entry_point_label);
        is_entry_function = true;
    } else {
        func->label = put_label_here();
    }

    parser.current_function_label = func->label;

    uint8_t param_registers[UINT8_MAX];

    consume(TOKEN_LEFT_PAREN, "Expected '(' after function name");
    if (!check(TOKEN_RIGHT_PAREN)) {
        do {
            param_registers[func->arity++] = (uint8_t) parse_variable("Expected function parameter name");
        } while (match(TOKEN_COMMA));
    }
    consume(TOKEN_RIGHT_PAREN, "Expected ')' after function parameters");

    consume(TOKEN_LEFT_BRACE, "Expected '{' before function body");

    if (!is_entry_function) {
        // ret addr
        emit_stack_pop(reg);
    }

    for (size_t i = func->arity; i != 0; i--) {
        define_variable(param_registers[i - 1], &current->locals[current->local_count - (func->arity - i) - 1]);
    }

    if (!is_entry_function) {
        emit_stack_push(reg);
    }

    free_register(reg);

    block();

    end_scope();

    return func;
}

static void function_declaration(void) {
    consume(TOKEN_IDENTIFIER, "Expected function name");

    function();
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
        emit_stack_push_value(0);
    }

    consume(TOKEN_SEMICOLON, "Expected ';' after variable declaration");

    define_variable(reg, &current->locals[current->local_count - 1]);
}

static void expression_statement(void) {
    expression();
    consume(TOKEN_SEMICOLON, "Expected ';' after expression");

    int ret = alloc_register();
    if (ret < 0) {
        error_alloc_register();
    } else {
        uint8_t reg = (uint8_t) ret;
        emit_stack_pop(reg);

        free_register(reg);
    }
}

static void for_statement(void) {
    begin_scope();

    consume(TOKEN_LEFT_PAREN, "Expected '(' after 'for'");

    int ret = alloc_register();

    if (ret < 0) {
        error_alloc_register();
        return;
    }

    uint8_t reg = (uint8_t) ret;
    
    if (match(TOKEN_SEMICOLON)) {
        // No initializer
    } else if (match(TOKEN_LET)) {
        let_declaration();
    } else {
        expression_statement();
    }

    size_t exit_label = take_label();
    size_t start_label = put_label_here();

    if (!match(TOKEN_SEMICOLON)) {
        expression();
        consume(TOKEN_SEMICOLON, "Expected ';' after condition");

        emit_stack_pop(reg);

        emit_conditional_jump_ri(OP_JMPI_NEQ, reg, CONSTANT_FALSE, exit_label);
    }

    if (!match(TOKEN_RIGHT_PAREN)) {
        size_t body_label = take_label();
        emit_jump(body_label);

        size_t orig_start_label = start_label;
        start_label = put_label_here();

        expression();
        emit_stack_pop(reg);
        consume(TOKEN_RIGHT_PAREN, "Expected ')' after for clauses");

        emit_jump(orig_start_label);

        set_label_here(body_label);
    }

    statement();

    emit_jump(start_label);

    set_label_here(exit_label);

    free_register(reg);

    end_scope();
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

    emit_stack_pop(reg);

    emit_conditional_jump_ri(OP_JMPI_EQ, reg, CONSTANT_FALSE, else_label);

    statement();

    if (match(TOKEN_ELSE)) {
        size_t else_end_label = take_label();

        emit_jump(else_end_label);

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

        emit_stack_pop(reg);

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

static void return_statement(void) {
    if (match(TOKEN_SEMICOLON)) {
        emit_stack_push_value(0);

        if (parser.current_function_label != entry_point_label) {
            emit_stack_swap();
        }

        emit_opcode(OP_RET);
    } else {
        expression();
        consume(TOKEN_SEMICOLON, "Expected ';' after return value");

        if (parser.current_function_label != entry_point_label) {
            emit_stack_swap();
        }

        emit_opcode(OP_RET);
    }
}

static void while_statement(void) {
    size_t start_label = put_label_here();

    consume(TOKEN_LEFT_PAREN, "Expected '(' after 'while'");
    expression();
    consume(TOKEN_RIGHT_PAREN, "Expected ')' after condition");

    int ret = alloc_register();

    if (ret < 0) {
        error_alloc_register();
        return;
    }

    uint8_t reg = (uint8_t) ret;
    size_t exit_label = take_label();

    emit_stack_pop(reg);

    emit_conditional_jump_ri(OP_JMPI_EQ, reg, CONSTANT_FALSE, exit_label);

    statement();

    emit_jump(start_label);

    set_label_here(exit_label);

    free_register(reg);
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
    if (match(TOKEN_FUNCTION)) {
        function_declaration();
    } else if (match(TOKEN_LET)) {
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
    } else if (match(TOKEN_RETURN)) {
        return_statement();
    } else if (match(TOKEN_FOR)) {
        for_statement();
    } else if (match(TOKEN_WHILE)) {
        while_statement();
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
    parser.entry_function_exists = false;
    parser.labels_count = 0;
    parser.functions = NULL;
    parser.functions_count = 0;
    parser.functions_capacity = 0;
    
    parser.free_registers[0] = false;
    for (size_t i = 1; i < NUM_REGISTERS; i++) {
        parser.free_registers[i] = true;
    }

    emit_runtime();

    advance();
    
    while (!match(TOKEN_EOF)) {
        declaration();
    }

    end_compiler();

    if (!parser.entry_function_exists) {
        error("Missing entry function");
    }

    return !parser.had_error;
}
