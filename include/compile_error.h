#ifndef COMPILE_ERROR_H
#define COMPILE_ERROR_H

#include "token.h"
#include <stdexcept>
#include <string>

namespace Lox::Error {

extern bool had_compile_error;
struct CompileError;

void Log(int line, const std::string& message);
void Log(Token token, const std::string& message);
void Log(CompileError error);
void Report(int line, const std::string& where, const std::string& message);

struct CompileError : std::runtime_error {
  CompileError(Token token, const std::string& message)
      : std::runtime_error(message), token_(token) {}

  Token token_;
};

} // namespace Lox::Error

#endif // !COMPILE_ERROR_H
