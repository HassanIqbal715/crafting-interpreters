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

struct Break {
	Token name;
};

struct Expression {
	unique_ptr<Expr> expression;
};

struct If {
	unique_ptr<Expr> condition;
	unique_ptr<Stmt> thenBranch;
	unique_ptr<Stmt> elseBranch;
};

struct Print {
	unique_ptr<Expr> expression;
};

struct Var {
	Token name;
	unique_ptr<Expr> initializer;
};

struct While {
	unique_ptr<Expr> condition;
	unique_ptr<Stmt> body;
};

using StmtVariant = std::variant<Block, Break, Expression, If, Print, Var, 
		While>;

struct Stmt : StmtVariant {
	using StmtVariant::variant;
};
