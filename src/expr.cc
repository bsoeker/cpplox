#include "expr.h"
#include "token.h"
#include <memory>

AssignmentExpression::AssignmentExpression(Token variable_name, Expr value)
    : variable_name_(variable_name),
      value_(std::make_unique<Expr>(std::move(value))) {}

BinaryExpression::BinaryExpression(Expr left, Token op, Expr right)
    : left_(std::make_unique<Expr>(std::move(left))), op_(op),
      right_(std::make_unique<Expr>(std::move(right))) {}

GroupingExpression::GroupingExpression(Expr expr)
    : expr_(std::make_unique<Expr>(std::move(expr))) {}

LiteralExpression::LiteralExpression(LiteralValue value)
    : value_(std::move(value)) {}

UnaryExpression::UnaryExpression(Token op, Expr right)
    : op_(op), right_(std::make_unique<Expr>(std::move(right))) {}

VariableExpression::VariableExpression(Token variable_name)
    : variable_name_(variable_name) {}
