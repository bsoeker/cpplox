#ifndef BUFFER_H
#define BUFFER_H

#include <cstddef> // for size_t
#include <ostream>

class Buffer {
private:
  size_t size_;
  size_t cap_;
  int* data_;

  void resize(int);

public:
  // Constructors & destructor
  Buffer();                             // default ctor
  ~Buffer();                            // destructor
  Buffer(const Buffer&);                // copy ctor
  Buffer& operator=(const Buffer&);     // copy assignment
  Buffer(Buffer&&) noexcept;            // move ctor
  Buffer& operator=(Buffer&&) noexcept; // move assignment
  friend std::ostream& operator<<(std::ostream&, const Buffer&);

  void push_back(int);
  int pop_back();
  size_t getSize() const;
  bool isEmpty();
};

#endif // BUFFER_H
