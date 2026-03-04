package lox;

enum TokenType {
    // Single-character tokens.
    LEFT_PAREN, RIGHT_PAREN, LEFT_BRACE, RIGHT_BRACE, LEFT_SQUARE, RIGHT_SQUARE, 
    COMMA, DOT, SEMICOLON, SLASH, STAR, QUESTION, COLON, MODULO,

    // One or two character tokens.
    MINUS, MINUS_MINUS,
    PLUS, PLUS_PLUS,
    BANG, BANG_EQUAL,
    EQUAL, EQUAL_EQUAL,
    GREATER, GREATER_EQUAL,
    LESS, LESS_EQUAL,

    // Literals.
    IDENTIFIER, STRING, NUMBER,

    // Keywords.
    AND, CLASS, ELSE, FALSE, FUN, FOR, IF, NIL, OR, PRINT, RETURN, SUPER, THIS,
    TRUE, VAR, WHILE, DO, BREAK, CONTINUE,

    // End of file.
    EOF
};