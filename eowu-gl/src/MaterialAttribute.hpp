//
//  MaterialAttribute.hpp
//  eowu-gl
//
//  Created by Nick Fagan on 8/29/18.
//

#pragma once

#include "ShaderBuilder.hpp"
#include <mpark/variant.hpp>
#include <glm/glm.hpp>
#include <string>
#include <atomic>
#include <shared_mutex>
#include <mutex>
#include <utility>
#include <iostream>
#include <functional>

namespace eowu {
  template<typename ...T>
  class MaterialAttribute;
  
  template<typename ...T>
  using MaterialAttributeAggregateType = mpark::variant<T...>;
  
  template<typename T>
  using ConstraintType = std::function<typename std::decay<T>::type(typename std::decay<T>::type)>;
}

template<typename ...T>
class eowu::MaterialAttribute {
public:
  MaterialAttribute();
  MaterialAttribute(const eowu::MaterialAttribute<T...> &other);
  
  ~MaterialAttribute() = default;
  
  void SetName(const std::string &name);
  
  template<typename VT>
  void SetValue(const VT &value);
  
  template<typename VT>
  void SetValue(const VT &value, eowu::ConstraintType<VT> *constraint);
  
  template<typename VT>
  void SetContents(const std::string &name, const VT &value);
  
  bool TypeChanged() const;
  void NextFrame();
  
  std::string GetName() const;
  eowu::MaterialAttributeAggregateType<T...> GetValue() const;
  eowu::glsl::Types GetGLSLType() const;
  eowu::schema::GLSLIdentifier GetAttributeSchema() const;
  
private:
  mutable std::shared_mutex mut;
  
  std::string name;
  eowu::glsl::Types glsl_type;
  MaterialAttributeAggregateType<T...> value;
  
  std::atomic<bool> type_did_change;
};

//
//  impl
//

template<typename ...T>
eowu::MaterialAttribute<T...>::MaterialAttribute() {
  glsl_type = eowu::glsl::vec3;
  type_did_change = false;
}

template<typename ...T>
eowu::MaterialAttribute<T...>::MaterialAttribute(const eowu::MaterialAttribute<T...> &other) {
  glsl_type = other.GetGLSLType();
  name = other.GetName();
  value = other.GetValue();
  
  type_did_change = false;
}

template<typename ...T>
bool eowu::MaterialAttribute<T...>::TypeChanged() const {
  return type_did_change;
}

template<typename ...T>
void eowu::MaterialAttribute<T...>::NextFrame() {
  type_did_change = false;
}

template<typename ...T>
std::string eowu::MaterialAttribute<T...>::GetName() const {
  std::shared_lock<std::shared_mutex> lock(mut);
  
  return name;
}

template<typename ...T>
eowu::glsl::Types eowu::MaterialAttribute<T...>::GetGLSLType() const {
  std::shared_lock<std::shared_mutex> lock(mut);
  
  return glsl_type;
}

template<typename ...T>
eowu::MaterialAttributeAggregateType<T...> eowu::MaterialAttribute<T...>::GetValue() const {
  std::shared_lock<std::shared_mutex> lock(mut);
  
  return value;
}

template<typename ...T>
eowu::schema::GLSLIdentifier eowu::MaterialAttribute<T...>::GetAttributeSchema() const {
  auto name = GetName();
  auto type = GetGLSLType();
  
  return {name, type};
}

template<typename ...T>
void eowu::MaterialAttribute<T...>::SetName(const std::string &name) {
  std::unique_lock<std::shared_mutex> lock(mut);
  
  this->name = name;
}

template<typename ...T>
template<typename VT>
void eowu::MaterialAttribute<T...>::SetContents(const std::string &name, const VT &value) {
  SetName(name);
  SetValue(value);
}

template<typename ...T>
template<typename VT>
void eowu::MaterialAttribute<T...>::SetValue(const VT &value) {
  std::unique_lock<std::shared_mutex> lock(mut);
  
  auto current_index = this->value.index();
  
  this->value = value;
  this->glsl_type = eowu::glsl::TypeMap<VT>::value;
  this->type_did_change = this->value.index() != current_index;
}

template<typename ...T>
template<typename VT>
void eowu::MaterialAttribute<T...>::SetValue(const VT &value, eowu::ConstraintType<VT> *constraint) {
  std::unique_lock<std::shared_mutex> lock(mut);
  
  auto current_index = this->value.index();
  
  this->value = constraint(value);
  this->glsl_type = eowu::glsl::TypeMap<VT>::value;
  this->type_did_change = this->value.index() != current_index;
}
