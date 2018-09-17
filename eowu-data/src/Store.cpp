//
//  Store.cpp
//  eowu-common
//
//  Created by Nick Fagan on 9/8/18.
//

#include "Store.hpp"
#include "Error.hpp"

eowu::data::Store::Store() : is_bypassed(false) {}
eowu::data::Store::Store(bool is_bypassed_) : is_bypassed(is_bypassed_) {}

eowu::data::Store::~Store() {
  Close();
}

bool eowu::data::Store::IsOpen() const {
  return is_open.load();
}

bool eowu::data::Store::IsBypassed() const {
  return is_bypassed;
}

void eowu::data::Store::Open(const std::string &filename) {
  if (IsBypassed()) {
    return;
  }
  
  if (IsOpen()) {
    Close();
  }
  
  file.open(filename, std::ios::binary | std::ios::out);
  
  if (file.fail()) {
    throw eowu::FileIOError("Failed to open file: '" + filename + "'.");
  }
  
  is_open = true;
}

void eowu::data::Store::Close() {
  if (IsBypassed()) {
    return;
  }
  
  if (!is_open) {
    return;
  }
  
  file.close();
  is_open = false;
}

void eowu::data::Store::Write(const eowu::serialize::ByteArrayType &data) {
  if (IsBypassed()) {
    return;
  }
  
  if (!IsOpen()) {
    throw eowu::FileIOError("No file is open for writing.");
  }
  
  file.write(data.data(), data.size());
}
