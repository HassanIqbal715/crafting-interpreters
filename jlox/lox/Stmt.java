package lox;

import java.util.List;

abstract class Stmt {
	interface Visitor<R> {
		R visitArrStmt(Arr stmt);
		R visitBlockStmt(Block stmt);
		R visitBreakStmt(Break stmt);
		R visitClassStmt(Class stmt);
		R visitContinueStmt(Continue stmt);
		R visitDoStmt(Do stmt);
		R visitExpressionStmt(Expression stmt);
		R visitForStmt(For stmt);
		R visitFunctionStmt(Function stmt);
		R visitIfStmt(If stmt);
		R visitPrintStmt(Print stmt);
		R visitReturnStmt(Return stmt);
		R visitSwitchStmt(Switch stmt);
		R visitVarStmt(Var stmt);
		R visitWhileStmt(While stmt);
		R visitCommaDeclarationStmt(CommaDeclaration stmt);
	}

	static class Arr extends Stmt {
		Arr(Token name, Expr elements) {
			this.name = name;
			this.elements = elements;
		}

		@Override
		<R> R accept(Visitor<R> visitor) {
			return visitor.visitArrStmt(this);
		}

		final Token name;
		final Expr elements;
	}

	static class Block extends Stmt {
		Block(List<Stmt> statements) {
			this.statements = statements;
		}

		@Override
		<R> R accept(Visitor<R> visitor) {
			return visitor.visitBlockStmt(this);
		}

		final List<Stmt> statements;
	}

	static class Break extends Stmt {
		Break(Token keyword) {
			this.keyword = keyword;
		}

		@Override
		<R> R accept(Visitor<R> visitor) {
			return visitor.visitBreakStmt(this);
		}

		final Token keyword;
	}

	static class Class extends Stmt {
		Class(Token name, List<Stmt.Function> methods) {
			this.name = name;
			this.methods = methods;
		}

		@Override
		<R> R accept(Visitor<R> visitor) {
			return visitor.visitClassStmt(this);
		}

		final Token name;
		final List<Stmt.Function> methods;
	}

	static class Continue extends Stmt {
		Continue(Token keyword) {
			this.keyword = keyword;
		}

		@Override
		<R> R accept(Visitor<R> visitor) {
			return visitor.visitContinueStmt(this);
		}

		final Token keyword;
	}

	static class Do extends Stmt {
		Do(Expr condition, Stmt body) {
			this.condition = condition;
			this.body = body;
		}

		@Override
		<R> R accept(Visitor<R> visitor) {
			return visitor.visitDoStmt(this);
		}

		final Expr condition;
		final Stmt body;
	}

	static class Expression extends Stmt {
		Expression(Expr expression) {
			this.expression = expression;
		}

		@Override
		<R> R accept(Visitor<R> visitor) {
			return visitor.visitExpressionStmt(this);
		}

		final Expr expression;
	}

	static class For extends Stmt {
		For(Stmt initializer, Expr condition, Expr increment, Stmt body) {
			this.initializer = initializer;
			this.condition = condition;
			this.increment = increment;
			this.body = body;
		}

		@Override
		<R> R accept(Visitor<R> visitor) {
			return visitor.visitForStmt(this);
		}

		final Stmt initializer;
		final Expr condition;
		final Expr increment;
		final Stmt body;
	}

	static class Function extends Stmt {
		Function(Token name, List<Token> params, List<Stmt> body) {
			this.name = name;
			this.params = params;
			this.body = body;
		}

		@Override
		<R> R accept(Visitor<R> visitor) {
			return visitor.visitFunctionStmt(this);
		}

		final Token name;
		final List<Token> params;
		final List<Stmt> body;
	}

	static class If extends Stmt {
		If(Expr condition, Stmt thenBranch, Stmt elseBranch) {
			this.condition = condition;
			this.thenBranch = thenBranch;
			this.elseBranch = elseBranch;
		}

		@Override
		<R> R accept(Visitor<R> visitor) {
			return visitor.visitIfStmt(this);
		}

		final Expr condition;
		final Stmt thenBranch;
		final Stmt elseBranch;
	}

	static class Print extends Stmt {
		Print(Expr expression) {
			this.expression = expression;
		}

		@Override
		<R> R accept(Visitor<R> visitor) {
			return visitor.visitPrintStmt(this);
		}

		final Expr expression;
	}

	static class Return extends Stmt {
		Return(Token keyword, Expr value) {
			this.keyword = keyword;
			this.value = value;
		}

		@Override
		<R> R accept(Visitor<R> visitor) {
			return visitor.visitReturnStmt(this);
		}

		final Token keyword;
		final Expr value;
	}

	static class Switch extends Stmt {
		Switch(Expr value, List<SwitchCase> cases, SwitchCase defaultCase) {
			this.value = value;
			this.cases = cases;
			this.defaultCase = defaultCase;
		}

		@Override
		<R> R accept(Visitor<R> visitor) {
			return visitor.visitSwitchStmt(this);
		}

		final Expr value;
		final List<SwitchCase> cases;
		final SwitchCase defaultCase;
	}

	static class Var extends Stmt {
		Var(Token name, Expr initializer) {
			this.name = name;
			this.initializer = initializer;
		}

		@Override
		<R> R accept(Visitor<R> visitor) {
			return visitor.visitVarStmt(this);
		}

		final Token name;
		final Expr initializer;
	}

	static class While extends Stmt {
		While(Expr condition, Stmt body) {
			this.condition = condition;
			this.body = body;
		}

		@Override
		<R> R accept(Visitor<R> visitor) {
			return visitor.visitWhileStmt(this);
		}

		final Expr condition;
		final Stmt body;
	}

	static class CommaDeclaration extends Stmt {
		CommaDeclaration(List<Stmt> declarations) {
			this.declarations = declarations;
		}

		@Override
		<R> R accept(Visitor<R> visitor) {
			return visitor.visitCommaDeclarationStmt(this);
		}

		final List<Stmt> declarations;
	}

	abstract <R> R accept(Visitor<R> visitor);
}
