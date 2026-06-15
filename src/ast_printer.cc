#include "expr.h"
#include "utility.h"
#include <iostream>

// void PrintAST(const Expr& expr) {
//   std::visit(overloaded{[](const std::unique_ptr<BinaryExpression>& b) {
//                           std::cout << "(";
//                           PrintAST(*b->left_);
//                           std::cout << b->op_.lexeme() << " ";
//                           PrintAST(*b->right_);
//                           std::cout << ")";
//                         },
//                         [](const std::unique_ptr<GroupingExpression>& g) {
//                           std::cout << "(group ";
//                           PrintAST(*g->expr_);
//                           std::cout << ")";
//                         },
//                         [](const std::unique_ptr<LiteralExpression>& l) {
//                           std::visit(
//                               overloaded{[](Nil) { std::cout << "nil "; },
//                                          [](bool b) {
//                                            std::cout << std::boolalpha << b
//                                                      << " ";
//                                          },
//                                          [](const auto& value) {
//                                            std::cout << value << " ";
//                                          }},
//                               l->value_);
//                         },
//                         [](const std::unique_ptr<UnaryExpression>& u) {
//                           std::cout << "(";
//                           std::cout << u->op_.lexeme();
//                           PrintAST(*u->right_);
//                           std::cout << ")";
//                         },
//                         [](Nil) { std::cout << "[Invalid AST Node]"; }},
//              expr);
// }
