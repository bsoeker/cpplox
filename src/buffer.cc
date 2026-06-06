#include "buffer.h"
#include <algorithm>
#include <stdexcept>

Buffer::Buffer() : size_(0), cap_(10), data_(new int[cap_]) {}
Buffer::~Buffer() { delete[] data_; }
Buffer::Buffer(const Buffer& other)
    : size_(other.size_), cap_(other.cap_), data_(new int[other.cap_]) {
  std::copy(other.data_, other.data_ + other.size_, data_);
}
Buffer& Buffer::operator=(const Buffer& other) {
  if (this != &other) {
    delete[] data_;
    size_ = other.size_;
    cap_ = other.cap_;
    data_ = new int[cap_];
    std::copy(other.data_, other.data_ + other.size_, data_);
  }

  return *this;
}

Buffer::Buffer(Buffer&& other) noexcept
    : size_(other.size_), cap_(other.cap_), data_(other.data_) {
  other.data_ = nullptr;
  other.size_ = 0;
  other.cap_ = 0;
}

Buffer& Buffer::operator=(Buffer&& other) noexcept {
  if (this != &other) {
    delete[] data_;
    size_ = other.size_;
    cap_ = other.cap_;
    data_ = other.data_;
    other.data_ = nullptr;
    other.size_ = 0;
    other.cap_ = 0;
  }

  return *this;
}

std::ostream& operator<<(std::ostream& os, const Buffer& buf) {
  os << "[";
  for (size_t i = 0; i < buf.size_; i++) {
    os << buf.data_[i];
    if (i + 1 != buf.size_)
      os << ", ";
  }
  os << "]";

  return os;
}

void Buffer::resize(int new_cap) {
  int* new_data = new int[new_cap];
  std::copy(data_, data_ + size_, new_data);
  delete[] data_;
  data_ = new_data;
  cap_ = new_cap;
}

bool Buffer::isEmpty() { return size_ == 0; }

void Buffer::push_back(int value) {
  if (size_ == cap_)
    resize(cap_ * 2);

  data_[size_++] = value;
}
int Buffer::pop_back() {
  if (isEmpty())
    throw std::out_of_range("Buffer is empty!\n");

  return data_[--size_];
}

size_t Buffer::getSize() const { return size_; }
