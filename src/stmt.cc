#include "stmt.h"
#include "expr.h"
#include <memory>
#include <utility>

Expression::Expression(Expr expr)
    : expr_(std::make_unique<Expr>(std::move(expr))) {}

Print::Print(Expr expr) : expr_(std::make_unique<Expr>(std::move(expr))) {}
