package lox;

class RuntimeError extends RuntimeException {
    final Token token;

    RuntimeError(Token token, String message) {
        super((token.fileName != null && !token.fileName.isEmpty()) ? 
            "In '" + token.fileName + "'': " + message : message);
        this.token = token;
    }
}
