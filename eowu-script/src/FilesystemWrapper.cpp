//
//  FilesystemWrapper.cpp
//  eowu-common
//
//  Created by Nick Fagan on 10/1/18.
//

#include "FilesystemWrapper.hpp"
#include "Lua.hpp"
#include "Error.hpp"
#include "Constants.hpp"
#include "RuntimeUtil.hpp"
#include "parser/ParseUtil.hpp"
#include <eowu-common/fs.hpp>

void eowu::FilesystemWrapper::CreateLuaSchema(lua_State *L) {
  luabridge::getGlobalNamespace(L)
  .beginNamespace(eowu::constants::eowu_namespace)
  .beginNamespace(eowu::constants::eowu_fs_namespace)
  .addFunction("root", &eowu::FilesystemWrapper::get_eowu_root_directory)
  .addFunction("fullfile", &eowu::FilesystemWrapper::full_file)
  .endNamespace()
  .endNamespace();
}

std::string eowu::FilesystemWrapper::get_eowu_root_directory() {
  return eowu::fs::get_eowu_root_directory();
}

std::string eowu::FilesystemWrapper::full_file(lua_State *L) {
  const char* const func_id = "fullfile";
  
  int n_args = lua_gettop(L);
  
  if (n_args != 1) {
    auto msg = eowu::util::get_message_wrong_number_of_inputs(func_id, 1, n_args);
    throw eowu::LuaError(msg);
  }
  
  auto vec = eowu::parser::get_string_vector_from_state(L, -1);
  
  return eowu::fs::full_file(vec);
}
