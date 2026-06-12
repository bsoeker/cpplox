#include "parser.h"
#include "compile_error.h"
#include "expr.h"
#include "stmt.h"
#include "token_type.h"
#include <memory>
#include <vector>

Parser::Parser(std::vector<Token> tokens) : tokens_(tokens) {}

std::unique_ptr<std::vector<Stmt>> Parser::Parse() {
  auto statements = std::make_unique<std::vector<Stmt>>();
  while (!IsAtEnd()) {
    statements->push_back(ParseStatement());
  }

  return statements;
}

Expr Parser::ParseExpression() { return ParseEquality(); }

Stmt Parser::ParseStatement() {
  if (Match({TokenType::kPrint}))
    return PrintStatement();

  return ExpressionStatement();
}

Stmt Parser::PrintStatement() {
  Expr value = ParseExpression();
  Consume(TokenType::kSemicolon, "Expect ';' after value.");
  return std::make_unique<Print>(std::move(value));
}

Stmt Parser::ExpressionStatement() {
  Expr value = ParseExpression();
  Consume(TokenType::kSemicolon, "Expect ';' after value.");
  return std::make_unique<Print>(std::move(value));
}

Expr Parser::ParseEquality() {
  Expr expr = ParseComparison();
  while (Match({TokenType::kBangEqual, TokenType::kEqualEqual})) {
    Token op = Previous();
    Expr right = ParseComparison();
    expr = std::make_unique<BinaryExpression>(std::move(expr), op,
                                              std::move(right));
  }

  return expr;
}

Expr Parser::ParseComparison() {
  Expr expr = ParseTerm();
  while (Match({TokenType::kGreater, TokenType::kGreaterEqual, TokenType::kLess,
                TokenType::kLessEqual})) {
    Token op = Previous();
    Expr right = ParseTerm();
    expr = std::make_unique<BinaryExpression>(std::move(expr), op,
                                              std::move(right));
  }

  return expr;
}

Expr Parser::ParseTerm() {
  Expr expr = ParseFactor();
  while (Match({TokenType::kMinus, TokenType::kPlus})) {
    Token op = Previous();
    Expr right = ParseFactor();
    expr = std::make_unique<BinaryExpression>(std::move(expr), op,
                                              std::move(right));
  }

  return expr;
}

Expr Parser::ParseFactor() {
  Expr expr = ParseUnary();
  while (Match({TokenType::kStar, TokenType::kSlash})) {
    Token op = Previous();
    Expr right = ParseUnary();
    expr = std::make_unique<BinaryExpression>(std::move(expr), op,
                                              std::move(right));
  }

  return expr;
}

Expr Parser::ParseUnary() {
  if (Match({TokenType::kBang, TokenType::kMinus})) {
    Token op = Previous();
    Expr right = ParseUnary();
    return std::make_unique<::UnaryExpression>(op, std::move(right));
  }

  return ParsePrimary();
}

Expr Parser::ParsePrimary() {
  if (Match({TokenType::kFalse}))
    return std::make_unique<LiteralExpression>(false);
  if (Match({TokenType::kTrue}))
    return std::make_unique<LiteralExpression>(true);
  if (Match({TokenType::kNil}))
    return std::make_unique<LiteralExpression>(Nil{});
  if (Match({TokenType::kNumber, TokenType::kString}))
    return std::make_unique<LiteralExpression>(Previous().literal());
  if (Match({TokenType::kLeftParen})) {
    Expr expr = ParseExpression();
    Consume(TokenType::kRightParen, "Expect ')' after expression.");
    return std::make_unique<GroupingExpression>(std::move(expr));
  }

  throw Error(Peek(), "Expect expression.");
}

bool Parser::Match(const std::vector<TokenType>& types) {
  for (TokenType type : types) {
    if (Check(type)) {
      Advance();
      return true;
    }
  }

  return false;
}

bool Parser::Check(TokenType type) {
  if (IsAtEnd())
    return false;
  return Peek().type() == type;
}

bool Parser::IsAtEnd() { return Peek().type() == TokenType::kEof; }

Token Parser::Advance() {
  if (!IsAtEnd())
    current_++;
  return Previous();
}

Token Parser::Peek() { return tokens_.at(current_); }

Token Parser::Previous() { return tokens_.at(current_ - 1); }

Token Parser::Consume(TokenType type, std::string message) {
  if (Check(type))
    return Advance();

  throw Error(Peek(), message);
}

Lox::Error::CompileError Parser::Error(Token token, std::string message) {
  Lox::Error::Log(token, message);

  return Lox::Error::CompileError();
}

void Parser::Synchronize() {
  Advance();
  while (!IsAtEnd()) {
    if (Previous().type() == TokenType::kSemicolon)
      return;

    switch (Peek().type()) {
    case TokenType::kClass:
    case TokenType::kFun:
    case TokenType::kVar:
    case TokenType::kFor:
    case TokenType::kIf:
    case TokenType::kWhile:
    case TokenType::kPrint:
    case TokenType::kReturn:
      return;
    }

    Advance();
  }
}
