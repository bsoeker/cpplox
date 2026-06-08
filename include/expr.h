#ifndef EXPR_H
#define EXPR_H

#include "token.h"
#include <memory>
#include <variant>

struct BinaryExpression;
struct GroupingExpression;
struct LiteralExpression;
struct UnaryExpression;

using Nil = std::monostate;
using Expr = std::variant<
    Nil, std::unique_ptr<BinaryExpression>, std::unique_ptr<GroupingExpression>,
    std::unique_ptr<LiteralExpression>, std::unique_ptr<UnaryExpression>>;

struct BinaryExpression {
  BinaryExpression(Expr left, Token op, Expr right);

  std::unique_ptr<Expr> left_;
  Token op_;
  std::unique_ptr<Expr> right_;
};

struct GroupingExpression {
  GroupingExpression(Expr expr);

  std::unique_ptr<Expr> expr_;
};

struct LiteralExpression {
  LiteralExpression(LiteralValue value);

  LiteralValue value_;
};

struct UnaryExpression {
  UnaryExpression(Token op, Expr right);

  Token op_;
  std::unique_ptr<Expr> right_;
};

void PrintAST(const Expr& expr);

#endif // !EXPR_H
