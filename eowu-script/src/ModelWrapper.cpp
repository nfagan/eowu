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
#include "Util.hpp"
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

#define EOWU_VEC_FUNC_SETTER(name, setter, context) \
int eowu::ModelWrapper::name(lua_State *L) { \
  std::vector<double> nums; \
  try { \
    nums = eowu::parser::get_numeric_vector_from_state(L, -1); \
  } catch (const std::exception &e) { \
    throw eowu::LuaError(std::string(context) + "(): " + e.what()); \
  } \
  auto vec = eowu::util::require_vec3(nums); \
  auto &trans = model->GetTransform(); \
  trans.setter(vec); \
  return 0; \
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

int eowu::ModelWrapper::Draw(lua_State *L) {
  int inputs = lua_gettop(L);
  
  std::vector<std::string> window_ids;
  
  //  1, because user data is first input
  if (inputs == 1) {
    window_ids = window_container->Keys();
  } else {
    if (inputs != 2) {
      const std::string arg = std::to_string(inputs-1);
      throw eowu::LuaError("Invalid arguments to 'Draw()': Expected 1 or 0 inputs, got " + arg);
    }
    
    window_ids = eowu::parser::get_string_vector_from_state(L, -1);
  }
  
  for (const auto &win_id : window_ids) {
    renderer->Queue(*model.get(), window_container->Get(win_id));
  }
  
  return 0;
}

EOWU_VEC_FUNC_SETTER(SetPositionVector, SetPosition, "Position");
EOWU_VEC_FUNC_SETTER(SetRotationVector, SetRotation, "Rotation");
EOWU_VEC_FUNC_SETTER(SetScaleVector, SetScale, "Scale");

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

int eowu::ModelWrapper::SetColor(lua_State *L) {
  assert_material();
  auto nums = eowu::parser::get_numeric_vector_from_state(L, -1);
  auto vec = eowu::util::require_vec3(nums);
  model->GetMaterial()->SetFaceColor(vec);
  
  return 0;
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
  .addProperty("size", &eowu::ModelWrapper::GetScale, &eowu::ModelWrapper::SetScale)
  .addProperty("rotation", &eowu::ModelWrapper::GetRotation, &eowu::ModelWrapper::SetRotation)
  .addFunction("Texture", &eowu::ModelWrapper::SetTexture)
  .addFunction("Units", &eowu::ModelWrapper::SetUnits)
  .addCFunction("Draw", &eowu::ModelWrapper::Draw)
  .addCFunction("Position", &eowu::ModelWrapper::SetPositionVector)
  .addCFunction("Rotation", &eowu::ModelWrapper::SetRotationVector)
  .addCFunction("Size", &eowu::ModelWrapper::SetScaleVector)
  .addCFunction("Color", &eowu::ModelWrapper::SetColor)
  .endClass()
  .endNamespace();
}
