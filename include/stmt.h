#ifndef STMT_H
#define STMT_H

#include "expr.h"
#include <memory>
#include <variant>

struct Print;
struct Expression;

using Stmt
    = std::variant<Nil, std::unique_ptr<Expression>, std::unique_ptr<Print>>;

struct Expression {
  Expression(Expr expr);

  std::unique_ptr<Expr> expr_;
};
struct Print {
  Print(Expr expr);

  std::unique_ptr<Expr> expr_;
};

#endif // !STMT_H
