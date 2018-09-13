//
//  Commitable.cpp
//  eowu-common
//
//  Created by Nick Fagan on 9/8/18.
//

#include "Commitable.hpp"
#include "Error.hpp"
#include <cstddef>

eowu::data::Commitable::Commitable() :
is_committed(false) {
  //
}

eowu::data::Commitable::Commitable(const eowu::data::Commitable &other) :
is_committed(other.is_committed.load()), value(other.value) {
  //  
}

eowu::data::Commitable::Commitable(const eowu::data::Commitable &&other) noexcept :
is_committed(other.is_committed.load()), value(std::move(other.value)) {
  //
}

eowu::data::Commitable& eowu::data::Commitable::operator=(const eowu::data::Commitable &other) {
  eowu::data::Commitable tmp(other);
  *this = std::move(tmp);
  return *this;
}

eowu::data::Commitable& eowu::data::Commitable::operator=(const eowu::data::Commitable &&other) noexcept {
  is_committed = other.is_committed.load();
  value = std::move(other.value);
  
  return *this;
}

bool eowu::data::Commitable::IsCommitted() const {
  return is_committed;
}

void eowu::data::Commitable::Serialize(eowu::serialize::ByteArrayType &into) {
  std::unique_lock<std::mutex> lock(mut);
  
  eowu::serialize::serialize(value, into);
}

void eowu::data::Commitable::Use(const std::function<void(const eowu::data::Struct&)> &func) const {
  std::unique_lock<std::mutex> lock(mut);
  
  func(value);
}

void eowu::data::Commitable::Set(const eowu::data::Struct &value) {
  std::unique_lock<std::mutex> lock(mut);
  
  if (IsCommitted()) {
    throw eowu::AlreadyCommittedError("Attempted to commit an already committed value.");
  }
  
  this->value = value;
}

void eowu::data::Commitable::Uncommit() {
  std::unique_lock<std::mutex> lock(mut);
  
  is_committed = false;
}

void eowu::data::Commitable::Commit() {
  std::unique_lock<std::mutex> lock(mut);
  
  is_committed = true;
}

void eowu::data::Commitable::Reset() {
  is_committed = false;
}
