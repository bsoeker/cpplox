#include "interpreter.h"
#include "expr.h"
#include "runtime_error.h"
#include "token.h"
#include "token_type.h"
#include "utility.h"
#include <cassert>
#include <iostream>
#include <memory>
#include <string>
#include <system_error>
#include <utility>
#include <variant>

bool Interpreter::IsTruthy(const LiteralValue value) {
  return std::visit(overloaded{[](Nil) { return false; },
                               [](bool b) { return b; },
                               [](const auto&) { return true; }},
                    value);
}

LiteralValue Interpreter::Evaluate(const Expr& expr) {
  return std::visit(
      overloaded{
          [](const std::unique_ptr<LiteralExpression>& l) -> LiteralValue {
            return l->value_;
          },
          [this](const std::unique_ptr<GroupingExpression>& g) -> LiteralValue {
            return Evaluate(*g->expr_);
          },
          [this](const std::unique_ptr<UnaryExpression>& u) -> LiteralValue {
            LiteralValue right = Evaluate(*u->right_);

            if (u->op_.type() == TokenType::kMinus) {
              if (!std::holds_alternative<double>(right))
                throw Lox::Error::RuntimeError(u->op_,
                                               "Operand has to be a number!");

              return -std::get<double>(right);
            }

            else // u->op_.type() == TokenType::kBang
              return !IsTruthy(right);
          },
          [this](const std::unique_ptr<BinaryExpression>& b) -> LiteralValue {
            LiteralValue left = Evaluate(*b->left_);
            LiteralValue right = Evaluate(*b->right_);

            switch (b->op_.type()) {
            case TokenType::kMinus:
              return std::visit(
                  overloaded{
                      [](double l, double r) -> LiteralValue { return l - r; },
                      [&b](const auto&, const auto&) -> LiteralValue {
                        throw Lox::Error::RuntimeError(
                            b->op_, "Operands must be two numbers");
                      }},
                  left, right);

            case TokenType::kSlash:
              return std::visit(
                  overloaded{
                      [](double l, double r) -> LiteralValue { return l / r; },
                      [&b](const auto&, const auto&) -> LiteralValue {
                        throw Lox::Error::RuntimeError(
                            b->op_, "Operands must be two numbers");
                      }},
                  left, right);

            case TokenType::kStar:
              return std::visit(
                  overloaded{
                      [](double l, double r) -> LiteralValue { return l * r; },
                      [&b](const auto&, const auto&) -> LiteralValue {
                        throw Lox::Error::RuntimeError(
                            b->op_, "Operands must be two numbers");
                      }},
                  left, right);

            case TokenType::kPlus:
              return std::visit(
                  overloaded{
                      [](double l, double r) -> LiteralValue { return l + r; },
                      [](std::string l, std::string r) -> LiteralValue {
                        return l + r;
                      },
                      [&b](const auto&, const auto&) -> LiteralValue {
                        throw Lox::Error::RuntimeError(
                            b->op_,
                            "Operands must be two numbers or two strings");
                      }},
                  left, right);

            case TokenType::kGreater:
              return std::visit(
                  overloaded{
                      [](double l, double r) -> LiteralValue { return l > r; },
                      [&b](const auto&, const auto&) -> LiteralValue {
                        throw Lox::Error::RuntimeError(
                            b->op_, "Operands must be two numbers");
                      }},
                  left, right);

            case TokenType::kGreaterEqual:
              return std::visit(
                  overloaded{
                      [](double l, double r) -> LiteralValue { return l >= r; },
                      [&b](const auto&, const auto&) -> LiteralValue {
                        throw Lox::Error::RuntimeError(
                            b->op_, "Operands must be two numbers");
                      }},
                  left, right);

            case TokenType::kLess:
              return std::visit(
                  overloaded{
                      [](double l, double r) -> LiteralValue { return l < r; },
                      [&b](const auto&, const auto&) -> LiteralValue {
                        throw Lox::Error::RuntimeError(
                            b->op_, "Operands must be two numbers");
                      }},
                  left, right);

            case TokenType::kLessEqual:
              return std::visit(
                  overloaded{
                      [](double l, double r) -> LiteralValue { return l <= r; },
                      [&b](const auto&, const auto&) -> LiteralValue {
                        throw Lox::Error::RuntimeError(
                            b->op_, "Operands must be two numbers");
                      }},
                  left, right);

            case TokenType::kEqualEqual:
              return left == right;

            case TokenType::kBangEqual:
              return left != right;

            default:
              assert(false && "Forgot to implement a binary operator case!");
              std::unreachable();
            }
          },
          [](Nil) -> LiteralValue { return Nil{}; }

      },
      expr);
}

std::string Interpreter::Stringify(const LiteralValue value) {
  return std::visit(
      overloaded{
          [](Nil) -> std::string { return "nil"; },
          [](double d) {
            std::array<char, 32> buffer;
            auto [ptr, ec]
                = std::to_chars(buffer.data(), buffer.data() + buffer.size(), d,
                                std::chars_format::general);
            if (ec == std::errc{})
              return std::string(buffer.data(), ptr);

            return std::string("NaN");
          },
          [](bool b) -> std::string { return b ? "true" : "false"; },
          [](const std::string& s) -> std::string { return s; },
      },
      value);
}

void Interpreter::Interpret(const Expr& expr) {
  try {
    LiteralValue value = Evaluate(expr);
    std::cout << Stringify(value);
  } catch (Lox::Error::RuntimeError error) {
    Lox::Error::Log(error);
  }
}
