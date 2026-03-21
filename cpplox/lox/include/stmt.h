#pragma once
#include "token.h"
#include "expr.h"
#include <vector>
#include <variant>
#include <memory>

using namespace std;

struct Stmt;

using Object = std::variant<bool, double, std::string, std::monostate>;

struct Block {
	vector<unique_ptr<Stmt>> statements;
};

struct Expression {
	unique_ptr<Expr> expression;
};

struct Print {
	unique_ptr<Expr> expression;
};

struct Var {
	Token name;
	unique_ptr<Expr> initializer;
};

using StmtVariant = std::variant<Block, Expression, Print, Var>;

struct Stmt : StmtVariant {
	using StmtVariant::variant;
};
