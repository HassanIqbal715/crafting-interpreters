package lox;

import java.util.List;

abstract class Expr {
	interface Visitor<R> {
		R visitArrayExpr(Array expr);
		R visitAssignExpr(Assign expr);
		R visitAssignArrayExpr(AssignArray expr);
		R visitBinaryExpr(Binary expr);
		R visitCallExpr(Call expr);
		R visitGroupingExpr(Grouping expr);
		R visitLiteralExpr(Literal expr);
		R visitLogicalExpr(Logical expr);
		R visitUnaryExpr(Unary expr);
		R visitPostfixExpr(Postfix expr);
		R visitVariableExpr(Variable expr);
		R visitTernaryExpr(Ternary expr);
	}

	static class Array extends Expr {
		Array(Token name, Expr index) {
			this.name = name;
			this.index = index;
		}

		@Override
		<R> R accept(Visitor<R> visitor) {
			return visitor.visitArrayExpr(this);
		}

		final Token name;
		final Expr index;
	}

	static class Assign extends Expr {
		Assign(Token name, Expr value) {
			this.name = name;
			this.value = value;
		}

		@Override
		<R> R accept(Visitor<R> visitor) {
			return visitor.visitAssignExpr(this);
		}

		final Token name;
		final Expr value;
	}

	static class AssignArray extends Expr {
		AssignArray(Token name, Expr index, Expr value) {
			this.name = name;
			this.index = index;
			this.value = value;
		}

		@Override
		<R> R accept(Visitor<R> visitor) {
			return visitor.visitAssignArrayExpr(this);
		}

		final Token name;
		final Expr index;
		final Expr value;
	}

	static class Binary extends Expr {
		Binary(Expr left, Token operator, Expr right) {
			this.left = left;
			this.operator = operator;
			this.right = right;
		}

		@Override
		<R> R accept(Visitor<R> visitor) {
			return visitor.visitBinaryExpr(this);
		}

		final Expr left;
		final Token operator;
		final Expr right;
	}

	static class Call extends Expr {
		Call(Expr callee, Token paren, List<Expr> arguments) {
			this.callee = callee;
			this.paren = paren;
			this.arguments = arguments;
		}

		@Override
		<R> R accept(Visitor<R> visitor) {
			return visitor.visitCallExpr(this);
		}

		final Expr callee;
		final Token paren;
		final List<Expr> arguments;
	}

	static class Grouping extends Expr {
		Grouping(Expr expression) {
			this.expression = expression;
		}

		@Override
		<R> R accept(Visitor<R> visitor) {
			return visitor.visitGroupingExpr(this);
		}

		final Expr expression;
	}

	static class Literal extends Expr {
		Literal(Object value) {
			this.value = value;
		}

		@Override
		<R> R accept(Visitor<R> visitor) {
			return visitor.visitLiteralExpr(this);
		}

		final Object value;
	}

	static class Logical extends Expr {
		Logical(Expr left, Token operator, Expr right) {
			this.left = left;
			this.operator = operator;
			this.right = right;
		}

		@Override
		<R> R accept(Visitor<R> visitor) {
			return visitor.visitLogicalExpr(this);
		}

		final Expr left;
		final Token operator;
		final Expr right;
	}

	static class Unary extends Expr {
		Unary(Token operator, Expr right) {
			this.operator = operator;
			this.right = right;
		}

		@Override
		<R> R accept(Visitor<R> visitor) {
			return visitor.visitUnaryExpr(this);
		}

		final Token operator;
		final Expr right;
	}

	static class Postfix extends Expr {
		Postfix(Expr left, Token operator) {
			this.left = left;
			this.operator = operator;
		}

		@Override
		<R> R accept(Visitor<R> visitor) {
			return visitor.visitPostfixExpr(this);
		}

		final Expr left;
		final Token operator;
	}

	static class Variable extends Expr {
		Variable(Token name) {
			this.name = name;
		}

		@Override
		<R> R accept(Visitor<R> visitor) {
			return visitor.visitVariableExpr(this);
		}

		final Token name;
	}

	static class Ternary extends Expr {
		Ternary(Expr left, Token operator1, Expr mid, Token operator2, Expr right) {
			this.left = left;
			this.operator1 = operator1;
			this.mid = mid;
			this.operator2 = operator2;
			this.right = right;
		}

		@Override
		<R> R accept(Visitor<R> visitor) {
			return visitor.visitTernaryExpr(this);
		}

		final Expr left;
		final Token operator1;
		final Expr mid;
		final Token operator2;
		final Expr right;
	}

	abstract <R> R accept(Visitor<R> visitor);
}
