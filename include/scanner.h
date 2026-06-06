#ifndef SCANNER_H
#define SCANNER_H

#include "token.h"
#include "token_type.h"
#include <string_view>
#include <unordered_map>
#include <vector>

class Scanner {
public:
  explicit Scanner(std::string_view source);

  std::vector<Token> ScanTokens();

private:
  void ScanToken();
  void Identifier();
  void Number();
  void String();

  char Advance();
  char Peek() const;
  char PeekNext() const;

  bool Match(char expected);
  bool IsAtEnd() const;
  bool IsAlpha(char c) const;
  bool IsDigit(char c) const;
  bool IsAlphaNumeric(char c) const;

  void AddToken(TokenType type);
  void AddToken(TokenType type, LiteralValue literal);

  std::string_view source_;
  std::vector<Token> tokens_;
  int start_ = 0;
  int current_ = 0;
  int line_ = 1;
  static const std::unordered_map<std::string_view, TokenType> kKeywords;
};

#endif // !SCANNER_H
