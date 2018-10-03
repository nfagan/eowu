//
//  string.hh
//  eowu
//
//  Created by Nick Fagan on 10/3/18.
//

#pragma once

#include <numeric>

template<typename T>
std::string eowu::util::join(const T &v, const char* const delim) {
  //  https://stackoverflow.com/questions/5689003/how-to-implode-a-vector-of-strings-into-a-string-the-elegant-way
  return std::accumulate(v.begin(), v.end(), std::string(), [&](const std::string &a, const std::string &b) -> std::string {
    return a + (a.length() > 0 ? delim : "") + b;
  });
}
