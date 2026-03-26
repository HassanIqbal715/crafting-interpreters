#pragma once
#include "object.h"
#include "token.h"
#include <vector>
#include <variant>
#include <memory>

using namespace std;

struct Expr;

struct Assign;
struct Binary;
struct Call;
struct Get;
struct Grouping;
struct Literal;
struct Logical;
struct Set;
struct Ternary;
struct This;
struct Unary;
struct Variable;

struct Assign {
	Token name;
	unique_ptr<Expr> value;
};

struct Binary {
	unique_ptr<Expr> left;
	Token op;
	unique_ptr<Expr> right;
};

struct Call {
	unique_ptr<Expr> callee;
	Token paren;
	vector<unique_ptr<Expr>> arguments;
};

struct Get {
	unique_ptr<Expr> object;
	Token name;
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

struct Set {
	unique_ptr<Expr> object;
	Token name;
	unique_ptr<Expr> value;
};

struct Ternary {
	unique_ptr<Expr> left;
	Token op1;
	unique_ptr<Expr> mid;
	Token op2;
	unique_ptr<Expr> right;
};

struct This {
	Token keyword;
};

struct Unary {
	Token op;
	unique_ptr<Expr> right;
};

struct Variable {
	Token name;
};

using ExprVariant = std::variant<Assign, Binary, Call, Get, Grouping, Literal, 
		Logical, Set, Ternary, This, Unary, Variable>;

struct Expr : ExprVariant {
	using ExprVariant::variant;
};
