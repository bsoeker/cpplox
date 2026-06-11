#include "ast_printer.h"
#include "compile_error.h"
#include "expr.h"
#include "interpreter.h"
#include "parser.h"
#include "runtime_error.h"
#include "scanner.h"
#include <cstdlib>
#include <fstream>
#include <ios>
#include <iostream>
#include <stdexcept>
#include <string>
#include <type_traits>
#include <vector>

void run_prompt();
void run_file(const std::string&);
void run(const std::string&);

int main(int argc, char* argv[]) {
  if (argc > 2)
    std::cout << "usage: lox [script]";
  else if (argc == 2)
    run_file(argv[1]);
  else
    run_prompt();

  std::cout << std::decay_t<int>();
  return 0;
}

void run_file(const std::string& path) {
  std::ifstream file(path, std::ios::binary | std::ios::ate);

  if (!file.is_open())
    throw std::runtime_error("Failed to open file: " + path);

  std::streamsize size = file.tellg();
  std::vector<char> buffer(size);

  file.seekg(0, std::ios::beg);

  if (!file.read(buffer.data(), size))
    throw std::runtime_error("Failed to read file: " + path);

  run(std::string(buffer.begin(), buffer.end()));
  if (Lox::Error::had_compile_error)
    std::exit(65);
  if (Lox::Error::had_runtime_error)
    std::exit(70);
}

void run_prompt() {
  std::string line;

  for (;;) {
    std::cout << "lox> ";
    if (!std::getline(std::cin, line))
      break;

    run(line);
    Lox::Error::had_compile_error = false;
  }
}

void run(const std::string& source) {
  Scanner scanner(source);
  std::vector<Token> tokens = scanner.ScanTokens();

  Parser parser(tokens);
  Expr expression = parser.Parse();

  if (Lox::Error::had_compile_error)
    return;

  Interpreter interpreter;
  interpreter.Interpret(expression);
  std::cout << std::endl;
}
