//
//  ModelWrapper.cpp
//  eowu-script
//
//  Created by Nick Fagan on 9/1/18.
//

#include "ModelWrapper.hpp"
#include "VectorWrapper.hpp"
#include "Error.hpp"
#include "Lua.hpp"
#include <eowu-gl/eowu-gl.hpp>

#define EOWU_VEC_SETTER(name) \
void eowu::ModelWrapper::name(const eowu::VectorWrapper &val) {\
  auto model_ = model; \
  auto setter = [=]() -> void { \
    auto &trans = model_->GetTransform(); \
    trans.name(val.GetValue()); \
  }; \
  command_queue->Push(setter); \
}

#define EOWU_VEC_GETTER(name) \
eowu::VectorWrapper eowu::ModelWrapper::name() const { \
  const auto &trans = model->GetTransform(); \
  return trans.name(); \
}

eowu::ModelWrapper::ModelWrapper(std::shared_ptr<eowu::CommandQueue> command_queue, std::shared_ptr<eowu::Model> model) {
  this->command_queue = command_queue;
  this->model = model;
}

void eowu::ModelWrapper::assert_mesh() {
  if (!model->HasMesh()) {
    throw eowu::InvalidModelError("Model is invalid: missing mesh.");
  }
}

void eowu::ModelWrapper::assert_material() {
  if (!model->HasMaterial()) {
    throw eowu::InvalidModelError("Model is invalid: missing material.");
  }
}

EOWU_VEC_GETTER(GetScale);
EOWU_VEC_GETTER(GetPosition);
EOWU_VEC_GETTER(GetRotation);

EOWU_VEC_SETTER(SetPosition);
EOWU_VEC_SETTER(SetRotation);
EOWU_VEC_SETTER(SetScale);

void eowu::ModelWrapper::SetColor(double r, double b, double g) {
  assert_material();
  
  auto material_ = model->GetMaterial();
  
  auto setter = [=]() {
    material_->SetFaceColor(glm::vec3(r, g, b));
  };
  
  command_queue->Push(setter);
}

void eowu::ModelWrapper::CreateLuaSchema(lua_State *L) {
  luabridge::getGlobalNamespace(L)
  .beginNamespace("eowu")
  .beginClass<eowu::ModelWrapper>("_Model")
  .addProperty("position", &eowu::ModelWrapper::GetPosition, &eowu::ModelWrapper::SetPosition)
  .addFunction("Color", &eowu::ModelWrapper::SetColor)
  .endClass()
  .endNamespace();
}
