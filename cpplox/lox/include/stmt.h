#pragma once
#include "object.h"
#include "expr.h"
#include "token.h"
#include <vector>
#include <variant>
#include <memory>

using namespace std;

struct Stmt;

struct Block {
	vector<unique_ptr<Stmt>> statements;
};

struct Break {
	Token name;
};

struct Expression {
	unique_ptr<Expr> expression;
};

struct Function {
	Token name;
	vector<Token> params;
	vector<unique_ptr<Stmt>> body;
};

struct If {
	unique_ptr<Expr> condition;
	unique_ptr<Stmt> thenBranch;
	unique_ptr<Stmt> elseBranch;
};

struct Print {
	unique_ptr<Expr> expression;
};

struct Return {
	Token keyword;
	unique_ptr<Expr> value;
};

struct Var {
	Token name;
	unique_ptr<Expr> initializer;
};

struct While {
	unique_ptr<Expr> condition;
	unique_ptr<Stmt> body;
};

using StmtVariant = std::variant<Block, Break, Expression, Function, If, 
		Print, Return, Var, While>;

struct Stmt : StmtVariant {
	using StmtVariant::variant;
};
