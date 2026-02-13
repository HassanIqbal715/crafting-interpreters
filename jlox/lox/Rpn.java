package lox;

class Rpn implements Expr.Visitor<Void> {
    private final StringBuilder buffer = new StringBuilder();

    @Override
    public Void visitUnaryExpr(Expr.Unary expr) {
        expr.right.accept(this);
        pushBuffer(expr.operator.lexeme);
        return null;
    }

    @Override
    public Void visitBinaryExpr(Expr.Binary expr) {
        expr.left.accept(this);
        expr.right.accept(this);
        pushBuffer(expr.operator.lexeme);
        return null;
    }

    @Override
    public Void visitLiteralExpr(Expr.Literal expr) {
        pushBuffer(expr.value);
        return null;
    }

    @Override
    public Void visitGroupingExpr(Expr.Grouping expr) {
        expr.expression.accept(this);
        return null;
    }
    
    void pushBuffer(Object value) {
        buffer.append(value).append(" ");
    }

    public String print(Expr expr) {
        expr.accept(this);
        return buffer.toString().trim();
    }

    public static void main(String[] args) {
        Expr expression = new Expr.Binary(
            new Expr.Literal(1),
            new Token(TokenType.PLUS, "+", null, 1),
            new Expr.Binary(
                new Expr.Literal(2),
                new Token(TokenType.STAR, "*", null, 1),
                new Expr.Literal(3)
            )
        );
        System.out.println(new Rpn().print(expression));
    }
}
