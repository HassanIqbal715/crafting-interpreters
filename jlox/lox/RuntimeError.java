package lox;

class RuntimeError extends RuntimeException {
    final Token token;

    RuntimeError(Token token, String message, String fileName) {
        super((fileName != null && !fileName.isEmpty()) ? 
            "In '" + fileName + "'': " + message : message);
        this.token = token;
    }
}
