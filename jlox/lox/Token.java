package lox;

class Token {
    final TokenType type;    
    final String lexeme;
    final Object literal; // value
    final int line; // for offset calculation
    final String fileName;

    Token(TokenType type, String lexeme, Object literal, int line, 
        String fileName) {
        this.type = type;
        this.lexeme = lexeme;
        this.literal = literal;
        this.line = line;
        this.fileName = fileName;
    }

    public String toString() {
        return type + " " + lexeme + " " + literal;
    }
}
