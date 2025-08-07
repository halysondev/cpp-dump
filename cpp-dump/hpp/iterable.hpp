/*
 * Copyright (c) 2023 Ryota Sasaki.
 *
 * This source code is licensed under the MIT license found in the LICENSE file in the root
 * directory of this source tree.
 */

#pragma once

#include <iterator>

namespace cpp_dump {

namespace _detail {

// Tag dispatching structures to avoid type conversion issues
struct priority_tag_low {};
struct priority_tag_high : priority_tag_low {};

template <typename T>
inline auto _iterable_begin(const T &t, priority_tag_high) -> decltype(begin(t)) {
  return begin(t);
}

template <typename T>
inline auto _iterable_begin(const T &t, priority_tag_low) -> decltype(std::begin(t)) {
  return std::begin(t);
}

template <typename T>
inline auto iterable_begin(const T &t) -> decltype(_iterable_begin(t, priority_tag_high{})) {
  return _iterable_begin(t, priority_tag_high{});
}

template <typename T>
inline auto _iterable_end(const T &t, priority_tag_high) -> decltype(end(t)) {
  return end(t);
}

template <typename T>
inline auto _iterable_end(const T &t, priority_tag_low) -> decltype(std::end(t)) {
  return std::end(t);
}

template <typename T>
inline auto iterable_end(const T &t) -> decltype(_iterable_end(t, priority_tag_high{})) {
  return _iterable_end(t, priority_tag_high{});
}

template <typename T>
inline bool is_empty_iterable(const T &t) {
  return !(iterable_begin(t) != iterable_end(t));
}

template <typename T>
inline auto _iterable_size(const T &t, priority_tag_high, priority_tag_high) -> decltype(size(t)) {
  return size(t);
}

template <typename T>
inline auto _iterable_size(const T &t, priority_tag_high, priority_tag_low) -> decltype(std::size(t)) {
  return std::size(t);
}

template <typename T>
inline auto _iterable_size(const T &t, priority_tag_low, priority_tag_low)
    -> decltype(std::distance(iterable_begin(t), iterable_end(t))) {
  return std::distance(iterable_begin(t), iterable_end(t));
}

template <typename T>
inline std::size_t _iterable_size(const T &t, ...) {
  std::size_t size = 0;
  // allow lvalue iterator
  auto &&begin = iterable_begin(t);
  auto &&end = iterable_end(t);
  for (; begin != end; ++begin) ++size;
  return size;
}

template <typename T>
inline std::size_t iterable_size(const T &t) {
  return static_cast<std::size_t>(_iterable_size(t, priority_tag_high{}, priority_tag_high{}));
}

template <typename It>
inline auto _iterator_advance(It &it, std::size_t n, priority_tag_high) 
    -> decltype(std::advance(it, n), void()) {
  std::advance(it, n);
}

template <typename It>
inline void _iterator_advance(It &it, std::size_t n, priority_tag_low) {
  for (; n > 0; --n) ++it;
}

template <typename It>
inline void iterator_advance(It &it, std::size_t n) {
  _iterator_advance(it, n, priority_tag_high{});
}

}  // namespace _detail

}  // namespace cpp_dump
