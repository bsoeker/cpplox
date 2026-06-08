#include "expr.h"
#include "token.h"
#include <ios>
#include <iostream>
#include <memory>
#include <variant>

BinaryExpression::BinaryExpression(Expr left, Token op, Expr right)
    : left_(std::make_unique<Expr>(std::move(left))), op_(op),
      right_(std::make_unique<Expr>(std::move(right))) {}

GroupingExpression::GroupingExpression(Expr expr)
    : expr_(std::make_unique<Expr>(std::move(expr))) {}

LiteralExpression::LiteralExpression(LiteralValue value)
    : value_(std::move(value)) {}

UnaryExpression::UnaryExpression(Token op, Expr right)
    : op_(op), right_(std::make_unique<Expr>(std::move(right))) {}

// Implementation of the Visitor pattern using std::visit
template <typename... Ts> struct overloaded : Ts... {
  using Ts::operator()...;
};

void PrintAST(const Expr& expr) {
  std::visit(overloaded{[](const std::unique_ptr<BinaryExpression>& b) {
                          std::cout << "(";
                          PrintAST(*b->left_);
                          std::cout << b->op_.lexeme() << " ";
                          PrintAST(*b->right_);
                          std::cout << ")";
                        },
                        [](const std::unique_ptr<GroupingExpression>& g) {
                          std::cout << "(group ";
                          PrintAST(*g->expr_);
                          std::cout << ")";
                        },
                        [](const std::unique_ptr<LiteralExpression>& l) {
                          std::visit(
                              overloaded{[](Nil) { std::cout << "nil "; },
                                         [](bool b) {
                                           std::cout << std::boolalpha << b
                                                     << " ";
                                         },
                                         [](const auto& value) {
                                           std::cout << value << " ";
                                         }},
                              l->value_);
                        },
                        [](const std::unique_ptr<UnaryExpression>& u) {
                          std::cout << "(";
                          std::cout << u->op_.lexeme();
                          PrintAST(*u->right_);
                          std::cout << ")";
                        },
                        [](Nil) { std::cout << "[Invalid AST Node]"; }},
             expr);
}
