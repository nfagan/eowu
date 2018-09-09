//
//  Store.cpp
//  eowu-common
//
//  Created by Nick Fagan on 9/8/18.
//

#include "Store.hpp"
#include "Error.hpp"

eowu::data::Store::~Store() {
  Close();
}

bool eowu::data::Store::IsOpen() const {
  return is_open.load();
}

void eowu::data::Store::Open(const std::string &filename) {
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
  if (!is_open) {
    return;
  }
  
  file.close();
  is_open = false;
}

void eowu::data::Store::Write(const eowu::serialize::ByteArrayType &data) {
  if (!IsOpen()) {
    throw eowu::FileIOError("No file is open for writing.");
  }
  
  file.write(data.data(), data.size());
}
