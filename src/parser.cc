#include "parser.h"
#include "compile_error.h"
#include "expr.h"
#include "stmt.h"
#include "token_type.h"
#include <memory>
#include <variant>
#include <vector>

Parser::Parser(std::vector<Token> tokens) : tokens_(tokens) {}

std::vector<Stmt> Parser::Parse() {
  auto statements = std::vector<Stmt>();
  while (!IsAtEnd())
    statements.push_back(ParseDeclaration());

  return statements;
}

Stmt Parser::ParseDeclaration() {
  try {
    if (Match(TokenType::kVar))
      return ParseVarDeclaration();

    return ParseStatement();
  } catch (Lox::Error::CompileError error) {
    Lox::Error::Log(error);
    Synchronize();
    return Invalid{};
  }
}

Stmt Parser::ParseVarDeclaration() {
  Token variable_name
      = Consume(TokenType::kIdentifier, "Expect variable name.");

  Expr initializer;
  if (Match(TokenType::kEqual))
    initializer = ParseExpression();

  Consume(TokenType::kSemicolon, "Expect ';' after variable declaration.");
  return std::make_unique<VariableDeclaration>(variable_name,
                                               std::move(initializer));
}

Stmt Parser::ParseStatement() {
  if (Match(TokenType::kPrint))
    return ParsePrintStatement();

  return ParseExpressionStatement();
}

Stmt Parser::ParsePrintStatement() {
  Expr expr = ParseExpression();
  Consume(TokenType::kSemicolon, "Expect ';' after value.");
  return std::make_unique<Print>(std::move(expr));
}

Stmt Parser::ParseExpressionStatement() {
  Expr expr = ParseExpression();
  Consume(TokenType::kSemicolon, "Expect ';' after value.");
  return std::make_unique<Expression>(std::move(expr));
}

Expr Parser::ParseExpression() { return ParseAssignment(); }

Expr Parser::ParseAssignment() {
  Expr expr = ParseEquality();

  if (Match(TokenType::kEqual)) {
    Token equals = Previous();
    Expr value = ParseAssignment();

    if (std::holds_alternative<std::unique_ptr<VariableExpression>>(expr)) {
      Token variable_name
          = std::get<std::unique_ptr<VariableExpression>>(expr)->variable_name_;
      return std::make_unique<AssignmentExpression>(variable_name,
                                                    std::move(value));
    }

    throw Lox::Error::CompileError(equals, "Invalid assignment target.");
  }

  return expr;
}

Expr Parser::ParseEquality() {
  Expr expr = ParseComparison();
  while (Match(TokenType::kBangEqual, TokenType::kEqualEqual)) {
    Token op = Previous();
    Expr right = ParseComparison();
    expr = std::make_unique<BinaryExpression>(std::move(expr), op,
                                              std::move(right));
  }

  return expr;
}

Expr Parser::ParseComparison() {
  Expr expr = ParseTerm();
  while (Match(TokenType::kGreater, TokenType::kGreaterEqual, TokenType::kLess,
               TokenType::kLessEqual)) {
    Token op = Previous();
    Expr right = ParseTerm();
    expr = std::make_unique<BinaryExpression>(std::move(expr), op,
                                              std::move(right));
  }

  return expr;
}

Expr Parser::ParseTerm() {
  Expr expr = ParseFactor();
  while (Match(TokenType::kMinus, TokenType::kPlus)) {
    Token op = Previous();
    Expr right = ParseFactor();
    expr = std::make_unique<BinaryExpression>(std::move(expr), op,
                                              std::move(right));
  }

  return expr;
}

Expr Parser::ParseFactor() {
  Expr expr = ParseUnary();
  while (Match(TokenType::kStar, TokenType::kSlash)) {
    Token op = Previous();
    Expr right = ParseUnary();
    expr = std::make_unique<BinaryExpression>(std::move(expr), op,
                                              std::move(right));
  }

  return expr;
}

Expr Parser::ParseUnary() {
  if (Match(TokenType::kBang, TokenType::kMinus)) {
    Token op = Previous();
    Expr right = ParseUnary();
    return std::make_unique<::UnaryExpression>(op, std::move(right));
  }

  return ParsePrimary();
}

Expr Parser::ParsePrimary() {
  if (Match(TokenType::kFalse))
    return std::make_unique<LiteralExpression>(false);
  if (Match(TokenType::kTrue))
    return std::make_unique<LiteralExpression>(true);
  if (Match(TokenType::kNil))
    return std::make_unique<LiteralExpression>(Nil{});
  if (Match(TokenType::kNumber, TokenType::kString))
    return std::make_unique<LiteralExpression>(Previous().literal());
  if (Match(TokenType::kIdentifier))
    return std::make_unique<VariableExpression>(Previous());
  if (Match(TokenType::kLeftParen)) {
    Expr expr = ParseExpression();
    Consume(TokenType::kRightParen, "Expect ')' after expression.");
    return std::make_unique<GroupingExpression>(std::move(expr));
  }

  throw Error(Peek(), "Expect expression.");
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
  return Lox::Error::CompileError(token, message);
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
