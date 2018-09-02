//
//  Model.hpp
//  eowu-gl
//
//  Created by Nick Fagan on 8/28/18.
//

#pragma once

#include "Mesh.hpp"
#include "Transform.hpp"
#include "Program.hpp"
#include <glm/glm.hpp>
#include <memory>
#include <shared_mutex>

namespace eowu {
  class Model;
  class Material;
}

class eowu::Model {
public:
  Model();
  Model(const eowu::Model &other);
  Model(std::shared_ptr<eowu::Mesh> mesh);
  Model(std::shared_ptr<eowu::Mesh> mesh, std::shared_ptr<eowu::Material> material);
  
  void NextFrame();
  
  void SetMesh(std::shared_ptr<eowu::Mesh> mesh);
  void SetMaterial(std::shared_ptr<eowu::Material> material);
  void SetTransform(const eowu::Transform &trans);
  
  bool HasMesh() const;
  bool HasMaterial() const;
  
  eowu::Transform& GetTransform();
  const eowu::Transform& GetTransform() const;
  std::shared_ptr<eowu::Material> GetMaterial() const;
  std::shared_ptr<eowu::Mesh> GetMesh() const;
  
  ~Model() = default;
private:
  mutable std::shared_mutex mut;
  
  std::shared_ptr<eowu::Mesh> mesh;
  std::shared_ptr<eowu::Material> material;
  
  eowu::Transform transform;
};
