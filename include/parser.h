#ifndef PARSER_H
#define PARSER_H

#include "compile_error.h"
#include "expr.h"
#include "token.h"
#include "token_type.h"
#include <string>
#include <vector>

class Parser {
public:
  explicit Parser(std::vector<Token> tokens);
  Expr Parse();

private:
  Expr ParseExpression();
  Expr ParseEquality();
  Expr ParseComparison();
  Expr ParseTerm();
  Expr ParseFactor();
  Expr ParseUnary();
  Expr ParsePrimary();

  bool Match(const std::vector<TokenType>& types);
  bool Check(TokenType type);
  bool IsAtEnd();
  Token Advance();
  Token Peek();
  Token Previous();

  Token Consume(TokenType type, std::string message);
  Lox::Error::CompileError Error(Token token, std::string message);
  void Synchronize();

  std::vector<Token> tokens_;
  int current_ = 0;
};

#endif // !PARSER_H
