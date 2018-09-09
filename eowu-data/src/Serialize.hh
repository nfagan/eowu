//
//  Serialize.hh
//  eowu
//
//  Created by Nick Fagan on 9/7/18.
//

#pragma once

#include <eowu-common/logging.hpp>
#include <cstring>

template<typename T>
void eowu::serialize::serialize(const std::vector<T> &value, eowu::serialize::ByteArrayType &into, eowu::u32 flag) {
  eowu::serialize::priv::array(value.data(), value.size(), into, flag);
}

template<typename T>
void eowu::serialize::priv::scalar(T value, eowu::serialize::ByteArrayType &into, eowu::u32 flag) {
  constexpr auto id = eowu::serialize::constants::flag_type<T>::value;
  const auto flags = id | flag;
  
  std::size_t current_size = into.size();
  constexpr std::size_t value_size = sizeof(T);
  
  into.resize(current_size + value_size + constants::flag_size);
  
  auto end = &into[current_size];
  
  std::memcpy(end, &flags, constants::flag_size);
  std::memcpy(end + constants::flag_size, &value, value_size);
}

template<typename T>
void eowu::serialize::priv::array(T *value, std::size_t len, eowu::serialize::ByteArrayType &into, eowu::u32 flag) {
  constexpr auto id = eowu::serialize::constants::flag_type<T>::value;
  const auto flags = constants::array | id | flag;
  constexpr std::size_t type_size = sizeof(T);
  constexpr std::size_t len_size = sizeof(eowu::u64);
  
  eowu::u64 current_size = into.size();
  std::size_t value_size = len * type_size;
  
  into.resize(current_size + value_size + len_size + constants::flag_size);
  
  auto end = &into[current_size];
  
  std::memcpy(end, &flags, constants::flag_size);
  std::memcpy(end + constants::flag_size, &len, len_size);
  std::memcpy(end + constants::flag_size + len_size, value, value_size);
}
