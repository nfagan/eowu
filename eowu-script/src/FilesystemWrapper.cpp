//
//  FilesystemWrapper.cpp
//  eowu-common
//
//  Created by Nick Fagan on 10/1/18.
//

#include "FilesystemWrapper.hpp"
#include "Lua.hpp"
#include "Constants.hpp"
#include <eowu-common/fs.hpp>

void eowu::FilesystemWrapper::CreateLuaSchema(lua_State *L) {
  luabridge::getGlobalNamespace(L)
  .beginNamespace(eowu::constants::eowu_namespace)
  .beginNamespace(eowu::constants::eowu_fs_namespace)
  .addFunction("root", &eowu::FilesystemWrapper::get_eowu_root_directory)
  .endNamespace()
  .endNamespace();
}

std::string eowu::FilesystemWrapper::get_eowu_root_directory() {
  return eowu::fs::get_eowu_root_directory();
}
