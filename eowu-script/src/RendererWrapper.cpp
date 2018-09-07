//
//  RendererWrapper.cpp
//  eowu-common
//
//  Created by Nick Fagan on 9/3/18.
//

#include "RendererWrapper.hpp"
#include "Lua.hpp"
#include "Util.hpp"
#include "parser/ParseUtil.hpp"
#include "Constants.hpp"
#include "LuaFunction.hpp"
#include <eowu-gl/eowu-gl.hpp>
#include <assert.h>

eowu::RendererWrapper::RendererWrapper(std::shared_ptr<eowu::Renderer> renderer_) :
renderer(renderer_) {
  //
}

int eowu::RendererWrapper::SetClearColor(lua_State *L) {
  auto nums = eowu::parser::get_numeric_vector_from_state(L, -1);
  auto vec = eowu::util::require_vec3(nums);
  
  renderer->SetClearColor(vec);
  
  return 0;
}

double eowu::RendererWrapper::Delta() {
  auto delta = renderer->Delta();
  return delta.count();
}

void eowu::RendererWrapper::CreateLuaSchema(lua_State *L) {
  luabridge::getGlobalNamespace(L)
  .beginNamespace(eowu::constants::eowu_namespace)
  .beginClass<eowu::RendererWrapper>("_Renderer")
  .addFunction("Delta", &eowu::RendererWrapper::Delta)
  .addCFunction("Color", &eowu::RendererWrapper::SetClearColor)
  .endClass()
  .endNamespace();
}
