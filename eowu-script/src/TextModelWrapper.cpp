//
//  TextModelWrapper.cpp
//  eowu-common
//
//  Created by Nick Fagan on 12/25/18.
//

#include "TextModelWrapper.hpp"
#include "Lua.hpp"
#include "Error.hpp"
#include "Constants.hpp"
#include "parser/ParseUtil.hpp"
#include "Util.hpp"
#include <eowu-gl/eowu-gl.hpp>

eowu::TextModelWrapper::TextModelWrapper(std::shared_ptr<eowu::TextModel> model_,
                                         std::shared_ptr<eowu::FontFace> font_face_,
                                         std::shared_ptr<eowu::Renderer> renderer_,
                                         std::shared_ptr<eowu::WindowContainerMap> window_container_) :
text_model(model_),
font_face(font_face_),
renderer(renderer_),
window_container(window_container_) {
  //
}

void eowu::TextModelWrapper::SetText(const std::string &text) {
  text_model->SetText(text);
}

int eowu::TextModelWrapper::SetPositionVector(lua_State *L) {
  std::vector<double> nums;
  
  try {
    nums = eowu::parser::get_numeric_vector_from_state(L, -1);
  } catch (const std::exception &e) {
    throw eowu::LuaError(std::string("SetPosition") + "(): " + e.what());
  }
  
  auto vec = eowu::util::require_vec3(nums);
  
  text_model->GetTransform().SetPosition(vec);
  
  return 0;
}

void eowu::TextModelWrapper::SetScale(double scale) {
  text_model->SetScale(scale);
}

void eowu::TextModelWrapper::SetUnits(const std::string &units) {
  text_model->GetTransform().SetUnits(eowu::units::get_units_from_string_label(units));
}

void eowu::TextModelWrapper::SetIsCentered(bool value) {
  text_model->SetIsCentered(value);
}

int eowu::TextModelWrapper::Draw(lua_State *L) {
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
    eowu::TextRenderer::DrawableComponent component{*text_model.get(), font_face};
    renderer->Queue(component, window_container->Get(win_id));
  }
  
  return 0;
}

int eowu::TextModelWrapper::SetColor(lua_State *L) {
  using eowu::util::require_vec3;
  
  auto nums = eowu::parser::get_numeric_vector_from_state(L, -1);
  text_model->GetMaterial()->SetFaceColor(require_vec3(nums));
  
  return 0;
}

void eowu::TextModelWrapper::CreateLuaSchema(lua_State *L) {
  luabridge::getGlobalNamespace(L)
  .beginNamespace(eowu::constants::eowu_namespace)
  .beginClass<eowu::TextModelWrapper>("_TextModel")
  .addFunction("Text", &eowu::TextModelWrapper::SetText)
  .addCFunction("Draw", &eowu::TextModelWrapper::Draw)
  .addCFunction("Color", &eowu::TextModelWrapper::SetColor)
  .addCFunction("Position", &eowu::TextModelWrapper::SetPositionVector)
  .addFunction("Scale", &eowu::TextModelWrapper::SetScale)
  .addFunction("Units", &eowu::TextModelWrapper::SetUnits)
  .addFunction("Center", &eowu::TextModelWrapper::SetIsCentered)
  .endClass()
  .endNamespace();
}
