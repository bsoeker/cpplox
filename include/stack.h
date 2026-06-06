#ifndef STACK_H
#define STACK_H

#include <cstddef>
#include <memory>
#include <ostream>
#include <stdexcept>

template <typename T> class Stack {
private:
  std::unique_ptr<T[]> data;
  size_t capacity;
  size_t size;

  void resize(int);

public:
  Stack();
  template <typename U>
  friend std::ostream& operator<<(std::ostream&, const Stack<U>&);

  void push(T);
  T pop();
  T peek() const;
  size_t getSize() const;
  bool isEmpty() const;
};

template <typename T>
Stack<T>::Stack() : data(std::make_unique<T[]>(10)), capacity(10), size(0) {}

template <typename T> size_t Stack<T>::getSize() const { return this->size; }
template <typename T> bool Stack<T>::isEmpty() const { return this->size == 0; }

template <typename T> void Stack<T>::resize(int new_cap) {
  std::unique_ptr<T[]> new_data = std::make_unique<T[]>(new_cap);

  for (size_t i = 0; i < this->size; i++) {
    new_data[i] = std::move(data[i]);
  }

  this->data = std::move(new_data);
  this->capacity = new_cap;
}

template <typename T> void Stack<T>::push(T value) {
  if (this->size == this->capacity) {
    this->resize(size * 2);
  }
  this->data[size++] = value;
}

template <typename T> T Stack<T>::peek() const {
  if (isEmpty())
    throw std::runtime_error("Stack is empty!");
  return this->data[size - 1];
}
template <typename T> T Stack<T>::pop() {
  if (isEmpty())
    throw std::runtime_error("Stack is empty!");

  return this->data[--size];
}

template <typename T>
std::ostream& operator<<(std::ostream& os, const Stack<T>& s) {
  os << "[";
  size_t i = 0;
  for (; i < s.size - 1; i++)
    os << s.data[i] << ", ";

  os << s.data[i] << "]";

  return os;
}

#endif // !STACK_H
