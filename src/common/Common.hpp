
#pragma once

#include <memory>

using namespace std;

template<typename T>
using uptr = unique_ptr<T>;

template<typename T>
using sptr = shared_ptr<T>;

template<typename T>
inline shared_ptr<T> u2sptr(unique_ptr<T> &rhs) {
  return shared_ptr<T>(move(rhs));
}

template<typename T>
inline shared_ptr<T> u2sptr(unique_ptr<T> &&rhs) {
  return shared_ptr<T>(move(rhs));
}
