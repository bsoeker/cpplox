#ifndef RUNTIME_ERROR_H
#define RUNTIME_ERROR_H

#include "token.h"
#include <stdexcept>
#include <string>

namespace Lox::Error {

extern bool had_runtime_error;

struct RuntimeError : public std::runtime_error {
  Token token;

  RuntimeError(Token token, const std::string& message)
      : std::runtime_error(message), token(token) {}
};

void Log(RuntimeError error);

} // namespace Lox::Error

#endif // !RUNTIME_ERROR_H
