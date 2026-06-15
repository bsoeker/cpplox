#ifndef STMT_H
#define STMT_H

#include "expr.h"
#include <memory>
#include <variant>

struct Invalid {};
struct Print;
struct Expression;
struct VariableDeclaration;

using Stmt
    = std::variant<Invalid, std::unique_ptr<Expression>, std::unique_ptr<Print>,
                   std::unique_ptr<VariableDeclaration>>;

struct Expression {
  Expression(Expr expr);

  Expr expr_;
};

struct Print {
  Print(Expr expr);

  Expr expr_;
};

struct VariableDeclaration {
  VariableDeclaration(Token variable_name, Expr initializer);

  Token variable_name_;
  Expr initializer_;
};

#endif // !STMT_H
