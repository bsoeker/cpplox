#ifndef TOKEN_H
#define TOKEN_H

#include "token_type.h"
#include <ostream>
#include <string>
#include <type_traits>
#include <variant>

using LiteralValue = std::variant<std::monostate, bool, double, std::string>;

class Token {
public:
  Token(TokenType type, std::string lexeme, LiteralValue literal, int line)
      : type_(type), lexeme_(lexeme), literal_(literal), line_(line) {}
  friend std::ostream& operator<<(std::ostream&, const Token&);
  const std::string& lexeme() const { return lexeme_; }
  TokenType type() const { return type_; }
  int line() const { return line_; }
  const LiteralValue& literal() const { return literal_; }

private:
  TokenType type_;
  std::string lexeme_;
  LiteralValue literal_;
  int line_;
};

inline std::ostream& operator<<(std::ostream& os, const Token& token) {
  os << "type: " << TokenTypeToString(token.type_) << " | lexeme: \""
     << token.lexeme_ << "\""
     << " | literal: ";

  std::visit(
      [&os](const auto& value) {
        using T = std::decay_t<decltype(value)>;
        if constexpr (std::is_same_v<T, std::monostate>)
          os << "nil";
        else if constexpr (std::is_same_v<T, bool>)
          os << (value ? "true" : "false");
        else if constexpr (std::is_same_v<T, double>)
          os << value;
        else // std::string
          os << "\"" << value << "\"";
      },
      token.literal_);

  os << " | line: " << token.line_;
  return os;
}

#endif // !TOKEN_H
