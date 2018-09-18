//
//  random.cpp
//  eowu-common
//
//  Created by Nick Fagan on 9/17/18.
//

#include "random.hpp"
#include <random>

std::string eowu::random::get_random_string(std::size_t N) {
  //  https://stackoverflow.com/questions/440133/how-do-i-create-a-random-alpha-numeric-string-in-c
  static auto& chrs = "0123456789"
  "abcdefghijklmnopqrstuvwxyz"
  "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
  
  thread_local static std::mt19937 rg{std::random_device{}()};
  thread_local static std::uniform_int_distribution<std::string::size_type> pick(0, sizeof(chrs) - 2);
  
  std::string s;
  
  s.reserve(N);
  
  while (N--) {
    s += chrs[pick(rg)];
  }
  
  return s;
}
