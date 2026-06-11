#include "expr.h"
#include "token.h"
#include <memory>

BinaryExpression::BinaryExpression(Expr left, Token op, Expr right)
    : left_(std::make_unique<Expr>(std::move(left))), op_(op),
      right_(std::make_unique<Expr>(std::move(right))) {}

GroupingExpression::GroupingExpression(Expr expr)
    : expr_(std::make_unique<Expr>(std::move(expr))) {}

LiteralExpression::LiteralExpression(LiteralValue value)
    : value_(std::move(value)) {}

UnaryExpression::UnaryExpression(Token op, Expr right)
    : op_(op), right_(std::make_unique<Expr>(std::move(right))) {}
