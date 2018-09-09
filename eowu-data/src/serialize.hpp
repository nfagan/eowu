//
//  Serialize.hpp
//  eowu-common
//
//  Created by Nick Fagan on 9/7/18.
//

#pragma once

#include "Types.hpp"
#include <vector>
#include <cstddef>
#include <string>

namespace eowu {
  namespace serialize {
    void serialize(double value, eowu::serialize::ByteArrayType &into, eowu::u32 flag = 0u);
    void serialize(bool value, eowu::serialize::ByteArrayType &into, eowu::u32 flag = 0u);
    void serialize(const std::string &value, eowu::serialize::ByteArrayType &into, eowu::u32 flag = 0u);
    void serialize(const char *value, std::size_t len, eowu::serialize::ByteArrayType &into, eowu::u32 flag = 0u);
    void serialize(double *value, std::size_t len, eowu::serialize::ByteArrayType &into, eowu::u32 flag = 0u);
    void serialize(const std::vector<std::string> &value, eowu::serialize::ByteArrayType &into, eowu::u32 flag = 0u);
    
    template<typename T>
    void serialize(const std::vector<T> &value, eowu::serialize::ByteArrayType &into, eowu::u32 flag = 0u);
    
    namespace priv {
      template<typename T>
      void scalar(T value, eowu::serialize::ByteArrayType &into, eowu::u32 flag = 0u);
      
      template<typename T>
      void array(T *value, std::size_t len, eowu::serialize::ByteArrayType &into, eowu::u32 flag = 0u);
      
      void nd_array(std::size_t len, eowu::serialize::ByteArrayType &into, eowu::u32 flag = 0u);
    }
  }
}

#include "Serialize.hh"
