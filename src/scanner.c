#include <stdio.h>
#include <string.h>
#include "common.h"
#include "scanner.h"

typedef struct {
    const char* start;
    const char* current;
    size_t line;
    size_t column;
} Scanner;

Scanner scanner;

void init_scanner(const char* source) {
    scanner.start = source;
    scanner.current = source;
    scanner.line = 1;
    scanner.column = 1;
}

static bool is_identifier_prefix(char c) {
    return (c >= 'a' && c <= 'z') ||
           (c >= 'A' && c <= 'Z') ||
            c == '_';
}

static bool is_digit(char c) {
    return c >= '0' && c <= '9';
}

static bool is_at_end(void) {
    return *scanner.current == '\0';
}

static char advance(void) {
    scanner.current++;
    return scanner.current[-1];
}

static char peek(void) {
    return *scanner.current;
}

static char peek_next(void) {
    if (is_at_end()) {
        return '\0';
    }

    return scanner.current[1];
}

static bool match(char expected) {
    if (is_at_end()) {
        return false;
    }

    if (*scanner.current != expected) {
        return false;
    }

    scanner.current++;
    return true;
}

static Token make_token(TokenType type) {
    Token token;

    token.type = type;
    token.start = scanner.start;
    token.length = (size_t) (scanner.current - scanner.start);
    token.line = scanner.line;
    token.column = scanner.column;

    return token;
}

static Token error_token(const char* message) {
    Token token;

    token.type = TOKEN_ERROR;
    token.start = message;
    token.length = strlen(message);
    token.line = scanner.line;
    token.column = scanner.column;

    return token;
}

static void skip_whitespace(void) {
    while (true) {
        char c = peek();

        switch (c) {
            case ' ':
            case '\r':
            case '\t':
                advance();
                break;
            case '\n':
                scanner.line++;
                advance();
                break;
            case '/':
                // Treat comments as whitespace
                if (peek_next() == '/') {
                    while (peek() != '\n' && !is_at_end()) {
                        advance();
                    }
                } else {
                    return;
                }
                break;
            default:
                return;
        }
    }
}

static TokenType check_keyword(size_t start, size_t length, const char* rest, TokenType type) {
    if ((size_t) (scanner.current - scanner.start) == start + length &&
           memcmp(scanner.start + start, rest, length) == 0) {
        return type;
    }

    return TOKEN_IDENTIFIER;
}

static TokenType identifier_type(void) {
    switch (scanner.start[0]) {
        case 'e': return check_keyword(1, 3, "lse", TOKEN_ELSE);
        case 'f':
            if (scanner.current - scanner.start > 1) {
                switch (scanner.start[1]) {
                    case 'a': return check_keyword(2, 3, "lse", TOKEN_FALSE);
                    case 'o': return check_keyword(2, 1, "r", TOKEN_FOR);
                    case 'u': return check_keyword(2, 6, "nction", TOKEN_FUNCTION);
                }
            }
            break;
        case 'i': return check_keyword(1, 1, "f", TOKEN_IF);
        case 'l': return check_keyword(1, 2, "et", TOKEN_LET);
        case 'n': return check_keyword(1, 3, "ull", TOKEN_NULL);
        case 'p': return check_keyword(1, 4, "rint", TOKEN_PRINT);
        case 'r': return check_keyword(1, 5, "eturn", TOKEN_RETURN);
        case 't': return check_keyword(1, 3, "rue", TOKEN_TRUE);
        case 'w': return check_keyword(1, 4, "hile", TOKEN_WHILE);
    }

    return TOKEN_IDENTIFIER;
}

static Token identifier(void) {
    while (is_identifier_prefix(peek()) || is_digit(peek())) {
        advance();
    }

    return make_token(identifier_type());
}

static Token number(void) {
    while (is_digit(peek())) {
        advance();
    }

    if (peek() == '.' && is_digit(peek_next())) {
        advance();

        while (is_digit(peek())) {
            advance();
        }

        return make_token(TOKEN_FLOAT);
    }

    return make_token(TOKEN_INTEGER);
}

static Token string(void) {
    while (peek() != '"' && !is_at_end()) {
        if (peek() == '\n') {
            scanner.line++;
        }

        advance();
    }

    if (is_at_end()) {
        return error_token("Unterminated string");
    }

    advance();
    return make_token(TOKEN_STRING);
}

Token scan_token() {
    skip_whitespace();

    scanner.start = scanner.current;

    if (is_at_end()) {
        return make_token(TOKEN_EOF);
    }

    char c = advance();

    if (is_identifier_prefix(c)) {
        return identifier();
    }

    if (is_digit(c)) {
        return number();
    }

    switch (c) {
        case '(': return make_token(TOKEN_LEFT_PAREN);
        case ')': return make_token(TOKEN_RIGHT_PAREN);
        case '{': return make_token(TOKEN_LEFT_BRACE);
        case '}': return make_token(TOKEN_RIGHT_BRACE);
        case ';': return make_token(TOKEN_SEMICOLON);
        case ',': return make_token(TOKEN_COMMA);
        case '-': return make_token(TOKEN_MINUS);
        case '+': return make_token(TOKEN_PLUS);
        case '/': return make_token(TOKEN_SLASH);
        case '*': return make_token(TOKEN_STAR);
        case '^': return make_token(TOKEN_CARET);
        case '%': return make_token(TOKEN_PERCENT);
        case '!':
            return make_token(
                match('=') ? TOKEN_BANG_EQUAL : TOKEN_BANG);
        case '=':
            return make_token(
                match('=') ? TOKEN_EQUAL_EQUAL : TOKEN_EQUAL);
        case '<':
            if (match('=')) {
                return make_token(TOKEN_LESS_EQUAL);
            } else if (match('<')) {
                return make_token(TOKEN_LESS_LESS);
            }
            break;
        case '>':
            if (match('=')) {
                return make_token(TOKEN_GREATER_EQUAL);
            } else if (match('>')) {
                return make_token(TOKEN_GREATER_GREATER);
            }
            break;
        case '"':
            return string();
    }

    return error_token("Unexpected character");
}
