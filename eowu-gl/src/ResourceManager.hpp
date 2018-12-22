//
//  ResourceManagerImpl.hpp
//  eowu-gl
//
//  Created by Nick Fagan on 9/1/18.
//

#pragma once

#include "Error.hpp"
#include <eowu-common/random.hpp>
#include <memory>
#include <unordered_map>
#include <tuple>
#include <string>
#include <shared_mutex>
#include <mutex>

namespace eowu {
  class Model;
  class Mesh;
  class Material;
  class ResourceManager;
  
  template <typename ...T>
  class ResourceManagerImpl;
  
  template <typename T>
  using ResourceMapType = std::unordered_map<std::string, std::shared_ptr<T>>;
}

template <typename ...T>
class eowu::ResourceManagerImpl {
public:
  ResourceManagerImpl() = default;
  ~ResourceManagerImpl() = default;
  
  template <typename Resource, typename ...A>
  std::shared_ptr<Resource> Create(const std::string &id, A... args);
  
  template <typename Resource>
  bool Has(const std::string &id) const;
  
  template <typename Resource>
  std::shared_ptr<Resource> Get(const std::string &id) const;
  
private:
  mutable std::shared_mutex mut;
  
  std::tuple<eowu::ResourceMapType<T>...> resources;
};

template <typename ...T>
template <typename Resource, typename ...A>
std::shared_ptr<Resource> eowu::ResourceManagerImpl<T...>::Create(const std::string &id, A... args) {
  std::unique_lock<std::shared_mutex> lock(mut);
  
  auto res = std::make_shared<Resource>(args...);
  
  auto &resource_container = std::get<eowu::ResourceMapType<Resource>>(resources);
  
  resource_container.emplace(id, res);
  
  return res;
}

template <typename ...T>
template <typename Resource>
bool eowu::ResourceManagerImpl<T...>::Has(const std::string &id) const {
  std::shared_lock<std::shared_mutex> lock(mut);
  
  const auto &resource_container = std::get<eowu::ResourceMapType<Resource>>(resources);
  const auto &it = resource_container.find(id);
  
  return it != resource_container.end();
}

template <typename ...T>
template <typename Resource>
std::shared_ptr<Resource> eowu::ResourceManagerImpl<T...>::Get(const std::string &id) const {
  std::shared_lock<std::shared_mutex> lock(mut);
  
  const auto &resource_container = std::get<eowu::ResourceMapType<Resource>>(resources);
  
  const auto it = resource_container.find(id);
  
  if (it == resource_container.end()) {
    throw eowu::NonexistentResourceError("No resource with id '" + id + "'.");
  }
  
  return it->second;
}

//
//  ResourceManager
//

class eowu::ResourceManager : public eowu::ResourceManagerImpl<eowu::Model, eowu::Mesh, eowu::Material> {};
