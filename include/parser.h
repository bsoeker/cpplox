#ifndef PARSER_H
#define PARSER_H

#include "compile_error.h"
#include "expr.h"
#include "stmt.h"
#include "token.h"
#include "token_type.h"
#include <string>
#include <type_traits>
#include <vector>

class Parser {
public:
  explicit Parser(std::vector<Token> tokens);
  std::vector<Stmt> Parse();

private:
  Stmt ParseDeclaration();
  Stmt ParseVarDeclaration();
  Stmt ParseStatement();
  Stmt ParsePrintStatement();
  Stmt ParseExpressionStatement();
  Expr ParseExpression();
  Expr ParseAssignment();
  Expr ParseEquality();
  Expr ParseComparison();
  Expr ParseTerm();
  Expr ParseFactor();
  Expr ParseUnary();
  Expr ParsePrimary();

  template <typename... Args>
    requires(std::is_same_v<Args, TokenType> && ...)
  bool Match(Args... types) {
    if ((Check(types) || ...)) {
      Advance();
      return true;
    }

    return false;
  }

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
