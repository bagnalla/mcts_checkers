#ifndef UTIL_H
#define UTIL_H

#include <algorithm>
#include <functional>
#include <vector>

namespace util
{
  // a -> [a] -> bool
  template <class T>
  bool elem(const T &x, const std::vector<T> &v)
  {
    return std::find(v.begin(), v.end(), x) != v.end();
  }

  // a -> [[a]] -> [[a]]
  // Given an element and a vector of vectors, return the vectors with
  // x prepended to each.
  template <class T>
  std::vector<std::vector<T>>
    prefix_vectors(const T &x, const std::vector<std::vector<T>> &l)
  {
    std::vector<std::vector<T>> result;
    for (auto it = l.begin(); it != l.end(); ++it) {
      std::vector<T> v;
      v.push_back(x);
      v.insert(v.end(), it->begin(), it->end());
      result.push_back(v);
    }
    return result;
  }

  // a -> [a]
  template <class T>
  std::vector<T> singleton(const T &x)
  {
    std::vector<T> v;
    v.push_back(x);
    return v;
  }

  // (a -> b) -> [a] -> [b]
  template <class S, class T>
  std::vector<T> fmap(const std::function<S(const T&)> &f,
		      const std::vector<S> &v) {
    std::vector<S> result();
    result.reserve(v.size());
    for (auto it = v.begin(); it != v.end(); ++it) {
      result.push_back(f(*it));
    }
    return result;
  }
}

#endif
