#ifndef ERROR_H
#define ERROR_H

#include "token.h"
#include <string>

namespace Lox {

extern bool had_error;

void LogError(int line, const std::string& message);
void LogError(Token token, const std::string& message);
void LogReport(int line, const std::string& where, const std::string& message);

} // namespace Lox

#endif // !ERROR_H
