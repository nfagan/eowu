//
//  Serialize.cpp
//  eowu-common
//
//  Created by Nick Fagan on 9/8/18.
//

#include "Serialize.hpp"
#include <cstddef>

void eowu::serialize::serialize(const std::vector<std::string> &value, eowu::serialize::ByteArrayType &into, eowu::u32 flag) {
  const eowu::u32 use_flag = flag | eowu::serialize::constants::flag_type<std::string>::value;
  
  eowu::serialize::priv::nd_array(value.size(), into, use_flag);
  
  for (const auto &str : value) {
    eowu::serialize::serialize(str, into, flag);
  }
}

void eowu::serialize::serialize(double value, eowu::serialize::ByteArrayType &into, eowu::u32 flag) {
  priv::scalar<double>(value, into, flag);
}

void eowu::serialize::serialize(bool value, eowu::serialize::ByteArrayType &into, eowu::u32 flag) {
  priv::scalar<eowu::u8>((eowu::u8)value, into, flag);
}

void eowu::serialize::serialize(const std::string &value, eowu::serialize::ByteArrayType &into, eowu::u32 flag) {
  priv::array<const char>(value.c_str(), value.length(), into, flag);
}

void eowu::serialize::serialize(const char *value, std::size_t len, eowu::serialize::ByteArrayType &into, eowu::u32 flag) {
  priv::array<const char>(value, len, into, flag);
}

void eowu::serialize::serialize(double *value, std::size_t len, eowu::serialize::ByteArrayType &into, eowu::u32 flag) {
  priv::array<double>(value, len, into, flag);
}

void eowu::serialize::priv::nd_array(std::size_t len, eowu::serialize::ByteArrayType &into, eowu::u32 flag) {
  const auto flags = constants::nd_array | flag;
  constexpr std::size_t len_size = sizeof(eowu::u64);
  
  eowu::u64 current_size = into.size();
  
  into.resize(current_size + len_size + constants::flag_size);
  
  auto end = &into[current_size];
  
  std::memcpy(end, &flags, constants::flag_size);
  std::memcpy(end + constants::flag_size, &len, len_size);
}
