#pragma once
#include "token.h"
#include <vector>
#include <variant>
#include <memory>

using namespace std;

struct Expr;

using Object = std::variant<bool, double, std::string, std::monostate>;

struct Assign {
	Token name;
	unique_ptr<Expr> value;
};

struct Binary {
	unique_ptr<Expr> left;
	Token op;
	unique_ptr<Expr> right;
};

struct Grouping {
	unique_ptr<Expr> expression;
};

struct Literal {
	Object value;
};

struct Logical {
	unique_ptr<Expr> left;
	Token op;
	unique_ptr<Expr> right;
};

struct Ternary {
	unique_ptr<Expr> left;
	Token op1;
	unique_ptr<Expr> mid;
	Token op2;
	unique_ptr<Expr> right;
};

struct Unary {
	Token op;
	unique_ptr<Expr> right;
};

struct Variable {
	Token name;
};

using ExprVariant = std::variant<Assign, Binary, Grouping, Literal, Logical, 
		Ternary, Unary, Variable>;

struct Expr : ExprVariant {
	using ExprVariant::variant;
};
