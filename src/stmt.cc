#include "stmt.h"
#include "expr.h"
#include <utility>

Expression::Expression(Expr expr) : expr_(std::move(expr)) {}

Print::Print(Expr expr) : expr_(std::move(expr)) {}

VariableDeclaration::VariableDeclaration(Token name, Expr initializer)
    : variable_name_(name), initializer_(std::move(initializer)) {}
