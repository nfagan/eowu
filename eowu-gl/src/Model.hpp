//
//  Model.hpp
//  eowu-gl
//
//  Created by Nick Fagan on 8/28/18.
//

#pragma once

#include "Mesh.hpp"
#include "Transform.hpp"
#include <glm/glm.hpp>
#include <memory>

namespace eowu {
  class Model;
}

class eowu::Model {
public:
  Model();
  Model(std::shared_ptr<eowu::Mesh> mesh);
  
  const eowu::Transform& GetTransform() const;
  
  ~Model() = default;
private:
  std::shared_ptr<eowu::Mesh> mesh;
  eowu::Transform transform;
};
