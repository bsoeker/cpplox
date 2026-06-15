#include "environment.h"
#include "runtime_error.h"
#include "token.h"
#include <string>

void Environment::Define(std::string name, LiteralValue value) {
  values[std::move(name)] = std::move(value);
}

void Environment::Assign(Token name, LiteralValue value) {
  if (values.contains(name.lexeme())) {
    values[name.lexeme()] = value;
    return;
  }

  throw Lox::Error::RuntimeError(name, "Can't assign to undefined variable!");
}

LiteralValue Environment::get(Token name) {
  if (values.contains(name.lexeme()))
    return values[name.lexeme()];

  throw Lox::Error::RuntimeError(name,
                                 "Undefined variable '" + name.lexeme() + "'.");
}
