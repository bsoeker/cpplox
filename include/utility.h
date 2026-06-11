#ifndef UTILITY_H
#define UTILITY_H

// Implementation of the Visitor pattern using std::visit
template <typename... Ts> struct overloaded : Ts... {
  using Ts::operator()...;
};

#endif // !UTILITY_H
