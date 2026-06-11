#ifndef COMPILE_ERROR_H
#define COMPILE_ERROR_H

#include "token.h"
#include <stdexcept>
#include <string>

namespace Lox::Error {

extern bool had_compile_error;

void Log(int line, const std::string& message);
void Log(Token token, const std::string& message);
void Report(int line, const std::string& where, const std::string& message);

struct CompileError : std::runtime_error {
  CompileError() : std::runtime_error("Lox compilation/parse error!") {}
};

} // namespace Lox::Error

#endif // !COMPILE_ERROR_H
