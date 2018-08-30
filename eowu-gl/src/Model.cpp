//
//  Model.cpp
//  eowu-gl
//
//  Created by Nick Fagan on 8/28/18.
//

#include "Model.hpp"
#include "Material.hpp"
#include <mutex>

eowu::Model::Model() {
  this->mesh = nullptr;
  this->material = nullptr;
}

eowu::Model::Model(const eowu::Model &other) {
  mesh = other.mesh;
  transform = other.transform;
  material = other.GetMaterial();
  mesh = other.GetMesh();
}

eowu::Model::Model(std::shared_ptr<eowu::Mesh> mesh) {
  this->mesh = mesh;
}

eowu::Model::Model(std::shared_ptr<eowu::Mesh> mesh, std::shared_ptr<eowu::Material> material) {
  this->mesh = mesh;
  this->material = material;
}

const eowu::Transform& eowu::Model::GetTransform() const {
  return transform;
}

eowu::Transform& eowu::Model::GetTransform() {
  return transform;
}

void eowu::Model::SetMesh(std::shared_ptr<eowu::Mesh> mesh) {
  std::unique_lock<std::shared_mutex> lock(mut);
  this->mesh = mesh;
}

void eowu::Model::SetMaterial(std::shared_ptr<eowu::Material> material) {
  std::unique_lock<std::shared_mutex> lock(mut);
  this->material = material;
}

void eowu::Model::SetTransform(const eowu::Transform &trans) {
  std::unique_lock<std::shared_mutex> lock(mut);
  this->transform = trans;
}

std::shared_ptr<eowu::Material> eowu::Model::GetMaterial() const {
  std::shared_lock<std::shared_mutex> lock(mut);
  return material;
}

std::shared_ptr<eowu::Mesh> eowu::Model::GetMesh() const {
  std::shared_lock<std::shared_mutex> lock(mut);
  return mesh;
}

void eowu::Model::NextFrame() {
  if (material) {
    material->NextFrame();
  }
}
