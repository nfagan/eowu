//
//  string.cpp
//  eowu-common
//
//  Created by Nick Fagan on 10/3/18.
//

#include "string.hpp"

bool eowu::util::ends_with(const std::string &str, const std::string &end) {
  //  https://stackoverflow.com/questions/874134/find-if-string-ends-with-another-string-in-c
  if (str.length() >= end.length()) {
    return (0 == str.compare(str.length() - end.length(), end.length(), end));
  } else {
    return false;
  }
}
