#include "compile_error.h"
#include "runtime_error.h"
#include "token.h"
#include <iostream>
#include <string>

namespace Lox::Error {

bool had_compile_error = false;
bool had_runtime_error = false;

void Log(int line, const std::string& message) {
  Report(line, "", message);
  had_compile_error = true;
}

void Log(Token token, const std::string& message) {
  if (token.type() == TokenType::kEof)
    Report(token.line(), " at end", message);
  else
    Report(token.line(), " at '" + token.lexeme() + "'", message);
}

void Log(RuntimeError error) {
  Log(error.token_, error.what());
  had_runtime_error = true;
}

void Log(CompileError error) {
  Log(error.token_, error.what());
  had_compile_error = true;
}

void Report(int line, const std::string& where, const std::string& message) {
  std::cerr << "[line " << line << "] Error" << where << ": " << message
            << "\n";
}

} // namespace Lox::Error
