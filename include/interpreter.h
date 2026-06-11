#ifndef INTERPRETER_H
#define INTERPRETER_H

#include "expr.h"
#include "token.h"
#include <string>

class Interpreter {
public:
  void Interpret(const Expr& expr);

private:
  LiteralValue Evaluate(const Expr& expr);
  bool IsTruthy(const LiteralValue value);
  std::string Stringify(const LiteralValue value);
};

#endif // !INTERPRETER_H
