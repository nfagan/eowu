//
//  test-util.hpp
//  eowu-script
//
//  Created by Nick Fagan on 9/1/18.
//

#include <iostream>
#include <cstddef>
#include <vector>
#include <cmath>

struct lua_State;

namespace luabridge {
  class LuaRef;
}

namespace util {
  
  template<typename T>
  T min(const std::vector<T> &value);
  
  template<typename T>
  T max(const std::vector<T> &value);
  
  template<typename T>
  T mean(const std::vector<T> &value);
  
  template<typename T>
  T std(const std::vector<T> &value);
  
  void insert_vector(lua_State *L);
  void run_lua_test_script(lua_State *L, const std::string &file);
  std::string get_lua_test_script_directory();
  luabridge::LuaRef get_global_from_script_with_trap(lua_State *L, const std::string &file, const std::string &name);
  
}

template<typename T>
T util::min(const std::vector<T> &value) {
  std::size_t sz = value.size();
  
  if (sz == 0) {
    return 0;
  }
  
  T min_ = value[0];
  
  for (std::size_t i = 1; i < sz; i++) {
    if (value[i] < min_) {
      min_ = value[i];
    }
  }
  
  return min_;
}

template<typename T>
T util::max(const std::vector<T> &value) {
  std::size_t sz = value.size();
  
  if (sz == 0) {
    return 0;
  }
  
  T max_ = value[0];
  
  for (std::size_t i = 1; i < sz; i++) {
    if (value[i] > max_) {
      max_ = value[i];
    }
  }
  
  return max_;
}

template<typename T>
T util::mean(const std::vector<T> &value) {
  std::size_t sz = value.size();
  
  T sum = 0;
  
  for (std::size_t i = 0; i < sz; i++) {
    sum += value[i];
  }
  
  return sum / sz;
}

template<typename T>
T util::std(const std::vector<T> &value) {
  std::size_t sz = value.size();
  
  T mn = util::mean(value);
  T sum = 0;
  
  for (std::size_t i = 0; i < sz; i++) {
    T diff = value[i] - mn;
    sum += (diff * diff);
  }
  
  sum /= sz;
  
  return std::sqrt(sum);
}
