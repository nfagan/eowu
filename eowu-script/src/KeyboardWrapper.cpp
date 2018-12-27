//
//  KeyboardWrapper.cpp
//  eowu-common
//
//  Created by Nick Fagan on 9/18/18.
//

#include "KeyboardWrapper.hpp"
#include "Lua.hpp"
#include "Error.hpp"
#include "Constants.hpp"
#include "data/conversion.hpp"
#include <eowu-gl/eowu-gl.hpp>

eowu::KeyboardWrapper::KeyboardWrapper(eowu::Keyboard &kb) : keyboard(kb) {
  //
}

bool eowu::KeyboardWrapper::WasPressed(const std::string &key) {
  assert_key_name(key);
  
  return keyboard.WasPressed(key);
}

bool eowu::KeyboardWrapper::IsDown(const std::string &key) const {
  assert_key_name(key);
  
  return keyboard.IsPressed(key);
}

void eowu::KeyboardWrapper::assert_key_name(const std::string &key) const {
  if (!eowu::Keyboard::IsKeyName(key)) {
    throw eowu::LuaError("Unrecognized key name: '" + key + "'.");
  }
}

int eowu::KeyboardWrapper::GetKeyNames(lua_State *L) {
  auto key_names = eowu::Keyboard::GetKeyNames();
  
  eowu::data::to_lua(L, key_names);
  
  return 1;
}

void eowu::KeyboardWrapper::CreateLuaSchema(lua_State *L) {
  luabridge::getGlobalNamespace(L)
  .beginNamespace(eowu::constants::eowu_namespace)
  .beginClass<eowu::KeyboardWrapper>("_Keyboard")
  .addFunction("Pressed", &eowu::KeyboardWrapper::WasPressed)
  .addFunction("Down", &eowu::KeyboardWrapper::IsDown)
  .addCFunction("Keys", &eowu::KeyboardWrapper::GetKeyNames)
  .endClass()
  .endNamespace();
}
