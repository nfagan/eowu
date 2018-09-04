//
//  RendererWrapper.cpp
//  eowu-common
//
//  Created by Nick Fagan on 9/3/18.
//

#include "RendererWrapper.hpp"
#include "Lua.hpp"
#include "Constants.hpp"
#include "LuaFunction.hpp"
#include "LuaReferenceContainer.hpp"
#include <eowu-gl/eowu-gl.hpp>
#include <assert.h>

eowu::RendererWrapper::RendererWrapper(std::shared_ptr<eowu::Renderer> renderer_,
                                       std::shared_ptr<eowu::LuaFunction> lua_render_function_) :
renderer(renderer_), lua_render_function(lua_render_function_) {
  //
}

int eowu::RendererWrapper::SetRenderFunction(lua_State *L) {
  assert(lua_render_function);
  
  luabridge::LuaRef func = luabridge::LuaRef::fromStack(L, -1);
  eowu::LuaReferenceContainer ref(func);
  lua_render_function->Set(ref);
  
  return 0;
}

void eowu::RendererWrapper::CreateLuaSchema(lua_State *L) {
  luabridge::getGlobalNamespace(L)
  .beginNamespace(eowu::constants::eowu_namespace)
  .beginClass<eowu::RendererWrapper>("_Renderer")
  .addCFunction("Render", &eowu::RendererWrapper::SetRenderFunction)
  .endClass()
  .endNamespace();
}
