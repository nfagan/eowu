//
//  WindowWrapper.cpp
//  eowu-common
//
//  Created by Nick Fagan on 10/7/18.
//

#include "WindowWrapper.hpp"
#include "Lua.hpp"
#include "Constants.hpp"
#include <eowu-gl/eowu-gl.hpp>

eowu::WindowWrapper::WindowWrapper(std::shared_ptr<eowu::Window> win) : window(win) {
  //
}

double eowu::WindowWrapper::GetWidth() const {
  return double(window->GetWidth());
}

double eowu::WindowWrapper::GetHeight() const {
  return double(window->GetHeight());
}

const std::string& eowu::WindowWrapper::GetAlias() const {
  return window->GetAlias();
}

void eowu::WindowWrapper::CreateLuaSchema(lua_State *L) {
  luabridge::getGlobalNamespace(L)
  .beginNamespace(eowu::constants::eowu_namespace)
  .beginClass<eowu::WindowWrapper>("_Window")
  .addProperty("width", &eowu::WindowWrapper::GetWidth)
  .addProperty("height", &eowu::WindowWrapper::GetHeight)
  .endClass()
  .endNamespace();
}
