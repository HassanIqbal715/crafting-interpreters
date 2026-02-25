package lox;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;
import static lox.TokenType.*;

class Scanner {
    private final String source;
    private final List<Token> tokens = new ArrayList<>();
    private int start = 0;
    private int current = 0;
    private int line = 1;

    private static final Map<String, TokenType> keywords;

    static {
        keywords = new HashMap<>();
        keywords.put("and",    AND);
        keywords.put("class",  CLASS);
        keywords.put("else",   ELSE);
        keywords.put("false",  FALSE);
        keywords.put("for",    FOR);
        keywords.put("fun",    FUN);
        keywords.put("if",     IF);
        keywords.put("nil",    NIL);
        keywords.put("or",     OR);
        keywords.put("print",  PRINT);
        keywords.put("return", RETURN);
        keywords.put("super",  SUPER);
        keywords.put("this",   THIS);
        keywords.put("true",   TRUE);
        keywords.put("var",    VAR);
        keywords.put("while",  WHILE);
        keywords.put("do",     DO);
    }

    Scanner(String source) {
        this.source = source;
    }

    List<Token> scanTokens() {
        while (!isAtEnd()) {
            // starting here
            start = current;
            scanToken();
        }

        // end of the line
        tokens.add(new Token(EOF, "", null, line));
        return tokens;
    }

    private void scanToken() {
        char c = advance();
        
        // single-character
        switch(c) {
            case '(': addToken(LEFT_PAREN); break;
            case ')': addToken(RIGHT_PAREN); break;
            case '{': addToken(LEFT_BRACE); break;
            case '}': addToken(RIGHT_BRACE); break;
            case '[': addToken(LEFT_SQUARE); break;
            case ']': addToken(RIGHT_SQUARE); break;
            case ',': addToken(COMMA); break;
            case '.': addToken(DOT); break;
            case ';': addToken(SEMICOLON); break;
            case '*': addToken(STAR); break;
            case '%': addToken(MODULO); break;
            case '?': addToken(QUESTION); break;
            case ':': addToken(COLON); break;
            case '-':
                addToken(match('-') ? MINUS_MINUS : MINUS);
                break;
            case '+':
                addToken(match('+') ? PLUS_PLUS : PLUS);
                break;
            case '!':
                addToken(match('=') ? BANG_EQUAL : BANG); 
                break;
            case '=':
                addToken(match('=') ? EQUAL_EQUAL: EQUAL);
                break;
            case '<':
                addToken(match('=') ? LESS_EQUAL : LESS);
                break;
            case '>':
                addToken(match('=') ? GREATER_EQUAL : 
                    GREATER);
                break;
            case '/':
                if (match('/')) {
                    // look till the end of the comment
                    while(peek() != '\n' && !isAtEnd()) advance();
                } else if (match('*')) {
                    multiLineComment();
                } else {
                    addToken(SLASH);
                }
                break;
            
            case ' ':
            case '\r':
            case '\t':
                break;
            
            case '\n':
                line++;
                break;
            
            case '"': string(); break;

            default:
                if (isDigit(c)) {
                    number();
                } else if (isAlpha(c)) {
                    identifier();
                }
                else {
                    Lox.error(line, "Unexpected character.");
                }
                break;
        }
    }    

    // Skip over multi-line comments
    private void multiLineComment() {
        while (!isAtEnd()) {
            // end of comment reached
            if (peek() == '*' && peekNext() == '/') {
                // adjust the pointer
                advance();
                advance();
                return;
            }

            if (peek() == '\n') line++; // increment for new line

            // incrementing current pointer to prevent infinite recursion.
            advance();
        }

        // end of file reached, but the comment did not end.
        if (isAtEnd()) {
            Lox.error(line, "Unterminated multi-line comment!");
            return;
        }
    }

    // Check for identifier
    private void identifier() {
        while (isAlphaNumeric(peek())) advance();

        String text = source.substring(start, current);
        TokenType type = keywords.get(text);
        if (type == null) type = IDENTIFIER; // not a keyword
        addToken(type);
    }

    // Checks for string literals
    private void string() {
        while(peek() != '"' && !isAtEnd()) {
            if (peek() == '\n') line++;
            advance();
        }

        if (isAtEnd()) {
            Lox.error(line, "Unterminated string!");
            return;
        }

        // ending quote
        advance();

        // get the literal value
        String value = source.substring(start + 1, current - 1);
        addToken(STRING, value);
    }

    // Checks for number literals
    private void number() {
        while(isDigit(peek())) advance();

        // decimal
        if (peek() == '.' && isDigit(peekNext())) {
            advance();

            while(isDigit(peek())) advance();
        }

        addToken(NUMBER, Double.parseDouble(
            source.substring(start, current)));
    }

    // Checks if the next(not really) character matches our expected character
    private boolean match(char expected) {
        if (isAtEnd()) return false;
        if (source.charAt(current) != expected) return false;

        current++;
        return true;
    }

    // Just checks the next letter. Does not increment current
    private char peek() {
        if (isAtEnd()) return '\0';
        return source.charAt(current);
    }

    private char peekNext() {
        if (current + 1 >= source.length()) return '\0';
        return source.charAt(current + 1);
    }

    // Check if character is a digit
    private boolean isDigit(char c) {
        return c >= '0' && c <= '9';
    }

    // Check if character is an alphabet or underscore
    private boolean isAlpha(char c) {
        return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_';
    }

    // Check if character is alpha numeric
    private boolean isAlphaNumeric(char c) {
        return isDigit(c) || isAlpha(c);
    }

    // check using length and pointer position
    private boolean isAtEnd() {
        return current >= source.length();
    }

    // get the next character
    private char advance() {
        return source.charAt(current++);
    }

    private void addToken(TokenType token) {
        addToken(token, null);
    }

    private void addToken(TokenType type, Object literal) {
        String text = source.substring(start, current);
        tokens.add(new Token(type, text, literal, line));
    }
}
