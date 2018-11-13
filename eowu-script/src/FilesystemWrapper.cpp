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
#include <eowu-common/types.hpp>
#include <eowu-common/string.hpp>

void eowu::FilesystemWrapper::CreateLuaSchema(lua_State *L) {
  luabridge::getGlobalNamespace(L)
  .beginNamespace(eowu::constants::eowu_namespace)
  .beginNamespace(eowu::constants::eowu_fs_namespace)
  .addFunction("root", &eowu::FilesystemWrapper::get_eowu_root_directory)
  .addFunction("fullfile", &eowu::FilesystemWrapper::full_file)
  .addCFunction("dir", &eowu::FilesystemWrapper::dir)
  .addCFunction("find", &eowu::FilesystemWrapper::find)
  .endNamespace()
  .endNamespace();
}

int eowu::FilesystemWrapper::find(lua_State *L) {
  const char* const func_id = "find";
  
  int n_args = lua_gettop(L);
  int path_index = -2;
  int ext_index = -1;
  
  bool is_recursive = false;
  
  if (n_args < 2) {
    throw eowu::LuaError(eowu::util::get_message_wrong_number_of_inputs(func_id, 2, n_args));
  } else if (n_args > 3) {
    throw eowu::LuaError(eowu::util::get_message_wrong_number_of_inputs(func_id, 3, n_args));
  }
  
  if (n_args == 3) {
    is_recursive = eowu::parser::get_bool_or_error_from_state(L, -1);
    path_index = -3;
    ext_index = -2;
  }
  
  auto paths = eowu::parser::require_string_vector_from_state(L, path_index);
  auto exts = eowu::parser::require_string_vector_from_state(L, ext_index);
  
  const int n_paths = paths.size();
  const int n_exts = exts.size();
  int idx = 1;
  
  std::vector<std::string> components{"", ""};
  
  lua_newtable(L);
  
  for (int i = 0; i < n_paths; i++) {
    auto res = eowu::fs::get_directory_contents(paths[i], is_recursive);
    const int n_results = res.size();
    
    for (int j = 0; j < n_results; j++) {
      const auto &info = res[j];
      
      components[0] = info.directory;
      components[1] = info.name;
      
      std::string full_path = eowu::fs::full_file(components);
      
      for (int k = 0; k < n_exts; k++) {
        const auto &ext = exts[k];
        
        bool criterion = false;
        
        if (ext == "folders") {
          criterion = info.is_directory;
        } else {
          criterion = eowu::util::ends_with(full_path, ext);
        }
        
        if (criterion) {
          lua_pushstring(L, full_path.c_str());
          lua_rawseti(L, -2, idx++);
        }
      }
    }
  }
  
  return 1;
}

int eowu::FilesystemWrapper::dir(lua_State *L) {
  const char* const func_id = "dir";
  
  int n_args = lua_gettop(L);
  int path_index = -1;
  
  bool is_recursive = false;
  
  if (n_args == 0) {
    throw eowu::LuaError(eowu::util::get_message_wrong_number_of_inputs(func_id, 1, n_args));
  } else if (n_args > 2) {
    throw eowu::LuaError(eowu::util::get_message_wrong_number_of_inputs(func_id, 2, n_args));
  } else if (n_args == 2) {
    is_recursive = eowu::parser::get_bool_or_error_from_state(L, -1);
    path_index = -2;
  }
  
  auto paths = eowu::parser::require_string_vector_from_state(L, path_index);
  const std::size_t n_paths = paths.size();
  
  lua_newtable(L);
  int idx = 1;
  
  for (std::size_t i = 0; i < n_paths; i++) {
    auto res = eowu::fs::get_directory_contents(paths[i], is_recursive);
    const int n_results = res.size();
    
    for (int j = 0; j < n_results; j++) {
      create_finfo_table(L, res[j]);
      
      lua_rawseti(L, -2, idx++);
    }
  }
  
  return 1;
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

void eowu::FilesystemWrapper::create_finfo_table(lua_State *L, const eowu::fs::finfo &info) {
  lua_createtable(L, 0, 3);
  
  lua_pushstring(L, info.name.c_str());
  lua_setfield(L, -2, "name");
  
  lua_pushstring(L, info.directory.c_str());
  lua_setfield(L, -2, "directory");
  
  lua_pushboolean(L, info.is_directory);
  lua_setfield(L, -2, "is_directory");
}
