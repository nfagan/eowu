//
//  Model.cpp
//  eowu-gl
//
//  Created by Nick Fagan on 8/28/18.
//

#include "Model.hpp"

eowu::Model::Model() {
  this->mesh = nullptr;
}

eowu::Model::Model(std::shared_ptr<eowu::Mesh> mesh) {
  this->mesh = mesh;
}

const eowu::Transform& eowu::Model::GetTransform() const {
  return transform;
}
