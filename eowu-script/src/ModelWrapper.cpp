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
#include "Constants.hpp"
#include "parser/ParseUtil.hpp"
#include <eowu-gl/eowu-gl.hpp>

#define EOWU_VEC_SETTER(name) \
void eowu::ModelWrapper::name(const eowu::VectorWrapper &val) {\
  auto &trans = model->GetTransform(); \
  trans.name(val.GetValue()); \
}

#define EOWU_VEC_GETTER(name) \
eowu::VectorWrapper eowu::ModelWrapper::name() const { \
  const auto &trans = model->GetTransform(); \
  return trans.name(); \
}

eowu::ModelWrapper::ModelWrapper(std::shared_ptr<eowu::Model> model,
                                 std::shared_ptr<eowu::Renderer> renderer,
                                 std::shared_ptr<eowu::WindowContainerMap> window_container,
                                 std::shared_ptr<eowu::TextureManager> texture_manager) {
  this->renderer = renderer;
  this->model = model;
  this->texture_manager = texture_manager;
  this->window_container = window_container;
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

void eowu::ModelWrapper::Draw(lua_State *L) {
//  luabridge::LuaRef ref = luabridge::LuaRef::fromStack(L, -1);
  renderer->Queue(*model.get());
}

EOWU_VEC_GETTER(GetScale);
EOWU_VEC_GETTER(GetPosition);
EOWU_VEC_GETTER(GetRotation);

EOWU_VEC_SETTER(SetPosition);
EOWU_VEC_SETTER(SetRotation);
EOWU_VEC_SETTER(SetScale);

void eowu::ModelWrapper::SetUnits(const std::string &units) {
  auto &trans = model->GetTransform();
  trans.SetUnits(eowu::units::get_units_from_string_label(units));
}

void eowu::ModelWrapper::SetColor(double r, double g, double b) {
  assert_material();
  model->GetMaterial()->SetFaceColor(glm::vec3(r, g, b));
}

void eowu::ModelWrapper::SetTexture(const std::string &id) {
  assert_material();
  const auto tex = texture_manager->Get(id);
  model->GetMaterial()->SetFaceColor(tex);
}

void eowu::ModelWrapper::CreateLuaSchema(lua_State *L) {
  luabridge::getGlobalNamespace(L)
  .beginNamespace(eowu::constants::eowu_namespace)
  .beginClass<eowu::ModelWrapper>("_Model")
  .addProperty("position", &eowu::ModelWrapper::GetPosition, &eowu::ModelWrapper::SetPosition)
  .addProperty("scale", &eowu::ModelWrapper::GetScale, &eowu::ModelWrapper::SetScale)
  .addProperty("rotation", &eowu::ModelWrapper::GetRotation, &eowu::ModelWrapper::SetRotation)
  .addFunction("Color", &eowu::ModelWrapper::SetColor)
  .addFunction("Texture", &eowu::ModelWrapper::SetTexture)
  .addFunction("Draw", &eowu::ModelWrapper::Draw)
  .addFunction("Units", &eowu::ModelWrapper::SetUnits)
  .endClass()
  .endNamespace();
}
