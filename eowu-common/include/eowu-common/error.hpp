//
//  error.hpp
//  eowu-gl
//
//  Created by Nick Fagan on 9/1/18.
//

#pragma once

#define EOWU_GENERIC_ERROR(name) \
  class name : public std::runtime_error { \
  public: \
  name(const std::string &msg) : std::runtime_error(msg) {} \
  } \
