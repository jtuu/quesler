#include <stdio.h>
#include <stdlib.h>
#include "common.h"
#include "compiler.h"
#include "scanner.h"
#include "opcodes.h"

#ifdef DEBUG
#include "debug.h"
#endif

typedef struct {
    Token current;
    Token previous;
    bool had_error;
    bool panic_mode;
    bool free_registers[NUM_REGISTERS];
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

typedef void (*ParseFn)(void);

typedef struct {
    ParseFn prefix;
    ParseFn infix;
    Precedence precedence;
} ParseRule;

Parser parser;

Chunk* compiling_chunk;

static void expression(void);
static void statement(void);
static void declaration(void);
static ParseRule* get_rule(TokenType type);
static void parse_precedence(Precedence precedence);

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
}

#undef EMIT

static void binary(void) {
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

static void literal(void) {
    switch (parser.previous.type) {
        case TOKEN_FALSE:
            emit_dword(CONSTANT_FALSE);
            break;
        case TOKEN_TRUE:
            emit_dword(CONSTANT_TRUE);
            break;
        default:
            return;
    }
}

static void grouping(void) {
    expression();
    consume(TOKEN_RIGHT_PAREN, "Expected ')' after expression");
}

static void number_integer(void) {
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

static void unary(void) {
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
  [TOKEN_EQUAL_EQUAL]   = {NULL,           binary, PREC_NONE},
  [TOKEN_GREATER]       = {NULL,           binary, PREC_NONE},
  [TOKEN_GREATER_EQUAL] = {NULL,           binary, PREC_NONE},
  [TOKEN_LESS]          = {NULL,           binary, PREC_NONE},
  [TOKEN_LESS_EQUAL]    = {NULL,           binary, PREC_NONE},
  [TOKEN_IDENTIFIER]    = {NULL,           NULL,   PREC_NONE},
  [TOKEN_STRING]        = {NULL,           NULL,   PREC_NONE},
  [TOKEN_INTEGER]       = {number_integer, NULL,   PREC_NONE},
  // [TOKEN_FLOAT]         = {number_float,   NULL,   PREC_NONE},
  [TOKEN_AMP_AMP]       = {NULL,           NULL,   PREC_NONE},
  [TOKEN_ELSE]          = {NULL,           NULL,   PREC_NONE},
  [TOKEN_FALSE]         = {literal,        NULL,   PREC_NONE},
  [TOKEN_FOR]           = {NULL,           NULL,   PREC_NONE},
  [TOKEN_FUNCTION]      = {NULL,           NULL,   PREC_NONE},
  [TOKEN_IF]            = {NULL,           NULL,   PREC_NONE},
  [TOKEN_NULL]          = {NULL,           NULL,   PREC_NONE},
  [TOKEN_PIPE_PIPE]     = {NULL,           NULL,   PREC_NONE},
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

    prefix_rule();

    while (precedence <= get_rule(parser.current.type)->precedence) {
        advance();
        ParseFn infix_rule = get_rule(parser.previous.type)->infix;
        infix_rule();
    }
}

static ParseRule* get_rule(TokenType type) {
    return &rules[type];
}

static void expression(void) {
    parse_precedence(PREC_ASSIGNMENT);
}

static void expression_statement() {
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

static void declaration(void) {
    statement();
}

static void statement(void) {
    if (match(TOKEN_PRINT)) {
        print_statement();
    } else {
        expression_statement();
    }
}

bool compile(const char* source, Chunk* chunk) {
    init_scanner(source);

    compiling_chunk = chunk;

    parser.had_error = false;
    parser.panic_mode = false;
    
    parser.free_registers[0] = false;
    for (size_t i = 1; i < NUM_REGISTERS; i++) {
        parser.free_registers[i] = true;
    }

    advance();
    
    while (!match(TOKEN_EOF)) {
        declaration();
    }

    end_compiler();

    return !parser.had_error;
}
