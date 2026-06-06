#include <iostream>
#include <string>

bool had_error = false;

void report(int line, const std::string& where, const std::string& message) {
  std::cout << "[line " << line << "] Error" << where << ": " << message;
}
void error(int line, const std::string& message) {
  report(line, "", message);
  had_error = true;
}
