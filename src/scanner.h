#ifndef quesler_scanner_h
#define quesler_scanner_h

typedef enum {
  // Single-character tokens
  TOKEN_LEFT_PAREN, TOKEN_RIGHT_PAREN,
  TOKEN_LEFT_BRACE, TOKEN_RIGHT_BRACE,
  TOKEN_COMMA, TOKEN_MINUS, TOKEN_PLUS,
  TOKEN_SEMICOLON, TOKEN_SLASH, TOKEN_STAR,
  TOKEN_CARET, TOKEN_PERCENT,

  // One or two character tokens
  TOKEN_BANG, TOKEN_BANG_EQUAL,
  TOKEN_EQUAL, TOKEN_EQUAL_EQUAL,
  TOKEN_GREATER, TOKEN_GREATER_EQUAL, TOKEN_GREATER_GREATER,
  TOKEN_LESS, TOKEN_LESS_EQUAL, TOKEN_LESS_LESS,
  TOKEN_AMP, TOKEN_AMP_AMP,
  TOKEN_PIPE, TOKEN_PIPE_PIPE,

  // Literals
  TOKEN_IDENTIFIER, TOKEN_STRING,
  TOKEN_INTEGER, TOKEN_FLOAT,

  // Keywords
  TOKEN_ELSE, TOKEN_FALSE,
  TOKEN_FOR, TOKEN_FUNCTION, TOKEN_IF, TOKEN_NULL,
  TOKEN_PRINT, TOKEN_RETURN,
  TOKEN_TRUE, TOKEN_LET, TOKEN_WHILE,

  TOKEN_ERROR,
  TOKEN_EOF
} TokenType;

typedef struct {
    TokenType type;
    const char* start;
    size_t length;
    size_t line;
    size_t column;
} Token;

void init_scanner(const char* source);
Token scan_token(void);

#endif
