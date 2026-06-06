#include "scanner.h"
#include "token_type.h"
#include <iostream>
#include <string_view>
#include <unordered_map>

const std::unordered_map<std::string_view, TokenType> Scanner::kKeywords
    = {{"and", TokenType::kAnd},       {"class", TokenType::kClass},
       {"else", TokenType::kElse},     {"false", TokenType::kFalse},
       {"for", TokenType::kFor},       {"fun", TokenType::kFun},
       {"if", TokenType::kIf},         {"nil", TokenType::kNil},
       {"or", TokenType::kOr},         {"print", TokenType::kPrint},
       {"return", TokenType::kReturn}, {"super", TokenType::kSuper},
       {"this", TokenType::kThis},     {"true", TokenType::kTrue},
       {"var", TokenType::kVar},       {"while", TokenType::kWhile}};

Scanner::Scanner(std::string_view source) : source_(source) {}

std::vector<Token> Scanner::ScanTokens() {
  while (!IsAtEnd()) {
    start_ = current_;
    ScanToken();
  }

  tokens_.emplace_back(TokenType::kEof, "", std::monostate{}, line_);

  return tokens_;
}

void Scanner::ScanToken() {
  char c = Advance();
  switch (c) {
  case '(':
    AddToken(TokenType::kLeftParen);
    break;
  case ')':
    AddToken(TokenType::kRightParen);
    break;
  case '{':
    AddToken(TokenType::kLeftBrace);
    break;
  case '}':
    AddToken(TokenType::kRightBrace);
    break;
  case ',':
    AddToken(TokenType::kComma);
    break;
  case '.':
    AddToken(TokenType::kDot);
    break;
  case '-':
    AddToken(TokenType::kMinus);
    break;
  case '+':
    AddToken(TokenType::kPlus);
    break;
  case ';':
    AddToken(TokenType::kSemicolon);
    break;
  case '*':
    AddToken(TokenType::kStar);
    break;

  case '!':
    AddToken(Match('=') ? TokenType::kBangEqual : TokenType::kBang);
    break;
  case '=':
    AddToken(Match('=') ? TokenType::kEqualEqual : TokenType::kEqual);
    break;
  case '>':
    AddToken(Match('=') ? TokenType::kGreaterEqual : TokenType::kGreater);
    break;
  case '<':
    AddToken(Match('=') ? TokenType::kLessEqual : TokenType::kLess);
    break;

  case '/':
    if (Match('/')) {
      while (Peek() != '\n' && !IsAtEnd())
        Advance();
    } else {
      AddToken(TokenType::kSlash);
    }
    break;

  case ' ':
  case '\t':
  case '\r':
    break;

  case '\n':
    line_++;
    break;

  case '"':
    String();
    break;

  default:
    if (IsDigit(c)) {
      Number();
    } else if (IsAlpha(c)) {
      Identifier();
    } else {
      std::cerr << "[" << line_ << "] Error: Unexpected character: " << c
                << "\n";
    }
    break;
  }
}

void Scanner::Identifier() {
  while (IsAlphaNumeric(Peek()))
    Advance();

  // Substrings with std::string_view are virtually free! No copying.
  std::string_view text = source_.substr(start_, current_ - start_);

  auto it = kKeywords.find(text);
  TokenType type
      = (it != kKeywords.end()) ? it->second : TokenType::kIdentifier;

  AddToken(type);
}

void Scanner::String() {
  while (Peek() != '"' && !IsAtEnd()) {
    if (Peek() == '\n')
      line_++;
    Advance();
  }

  if (IsAtEnd()) {
    std::cerr << "[" << line_ << "] Error: Unterminated string.\n";
    return;
  }

  Advance(); // The closing "

  // Extract text inside the quotes
  std::string value{source_.substr(start_ + 1, current_ - start_ - 2)};
  AddToken(TokenType::kString, value);
}

void Scanner::Number() {
  while (IsDigit(Peek()))
    Advance();

  if (Peek() == '.' && IsDigit(PeekNext())) {
    Advance(); // Consume '.'
    while (IsDigit(Peek()))
      Advance();
  }

  std::string num_str{source_.substr(start_, current_ - start_)};
  AddToken(TokenType::kNumber,
           std::stod(num_str)); // std::stod parses string to double
}

char Scanner::Advance() { return source_[current_++]; }
char Scanner::Peek() const { return IsAtEnd() ? '\0' : source_[current_]; }

char Scanner::PeekNext() const {
  if (current_ + 1 >= source_.length())
    return '\0';
  return source_[current_ + 1];
}

bool Scanner::Match(char expected) {
  if (IsAtEnd() || source_[current_] != expected)
    return false;
  current_++;
  return true;
}

bool Scanner::IsAtEnd() const { return current_ >= source_.length(); }

bool Scanner::IsAlpha(char c) const {
  return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_';
}

bool Scanner::IsDigit(char c) const { return c >= '0' && c <= '9'; }
bool Scanner::IsAlphaNumeric(char c) const { return IsAlpha(c) || IsDigit(c); }

void Scanner::AddToken(TokenType type) {
  AddToken(type, std::monostate{}); // Implicit "nil" representation
}

void Scanner::AddToken(TokenType type, LiteralValue literal) {
  std::string lexeme{source_.substr(start_, current_ - start_)};
  tokens_.emplace_back(type, std::move(lexeme), std::move(literal), line_);
}
