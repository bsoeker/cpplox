#include "error.h"
#include "token.h"
#include <iostream>
#include <string>

namespace Lox {

bool had_error = false;

void LogError(int line, const std::string& message) {
  LogReport(line, "", message);
  had_error = true;
}

void LogError(Token token, const std::string& message) {
  if (token.type() == TokenType::kEof)
    LogReport(token.line(), " at end", message);
  else
    LogReport(token.line(), " at '" + token.lexeme() + "'", message);

  had_error = true;
}

void LogReport(int line, const std::string& where, const std::string& message) {
  std::cerr << "[line " << line << "] Error" << where << ": " << message
            << "\n";
}
} // namespace Lox
