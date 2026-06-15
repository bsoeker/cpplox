#ifndef ENVIRONMENT_H
#define ENVIRONMENT_H

#include "token.h"
#include <string>
#include <unordered_map>
class Environment {
private:
  std::unordered_map<std::string, LiteralValue> values;

public:
  void Define(std::string name, LiteralValue value);
  void Assign(Token name, LiteralValue value);
  LiteralValue get(Token name);
};

#endif // !ENVIRONMENT_H
