#ifndef INTERPRETER_H
#define INTERPRETER_H

#include "environment.h"
#include "expr.h"
#include "stmt.h"
#include "token.h"
#include <string>
#include <vector>

class Interpreter {
public:
  void Interpret(const std::vector<Stmt>& statements);

private:
  LiteralValue Evaluate(const Expr& expr);
  bool IsTruthy(const LiteralValue value);
  std::string Stringify(const LiteralValue value);
  void Execute(const Stmt& stmt);

  Environment environment;
};

#endif // !INTERPRETER_H
