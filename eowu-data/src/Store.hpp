//
//  Store.hpp
//  eowu-common
//
//  Created by Nick Fagan on 9/8/18.
//

#pragma once

#include "Types.hpp"
#include <fstream>
#include <string>
#include <atomic>

namespace eowu {
  namespace data {
    class Store;
  }
}

class eowu::data::Store {
public:
  Store();
  Store(bool is_bypassed);
  ~Store();
  
  void Open(const std::string &filename);
  void Close();
  void Write(const eowu::serialize::ByteArrayType &data);
  bool IsOpen() const;
  bool IsBypassed() const;
  
private:
  bool is_bypassed;
  
  std::atomic<bool> is_open;
  
  std::ofstream file;
};
