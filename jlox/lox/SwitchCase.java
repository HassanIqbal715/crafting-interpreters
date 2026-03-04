package lox;

import java.util.List;

class SwitchCase {
    final Expr value;
    final List<Stmt> statements;

    SwitchCase(Expr value, List<Stmt> statements) {
        this.value = value;
        this.statements = statements;
    }
}
