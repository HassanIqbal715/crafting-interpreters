#pragma once
#include "token.h"
#include <vector>
#include <variant>
#include <memory>

struct Expr;

using Object = std::variant<bool, double, std::string, std::monostate>;

struct Binary {
	std::unique_ptr<Expr> left;
	Token op;
	std::unique_ptr<Expr> right;
};

struct Grouping {
	std::unique_ptr<Expr> expression;
};

struct Literal {
	Object value;
};

struct Ternary {
	std::unique_ptr<Expr> left;
	Token op1;
	std::unique_ptr<Expr> mid;
	Token op2;
	std::unique_ptr<Expr> right;
};

struct Unary {
	Token op;
	std::unique_ptr<Expr> right;
};

using ExprVariant = std::variant<Binary, Grouping, Literal, Ternary, Unary>;

struct Expr : ExprVariant {
	using ExprVariant::variant;
};
