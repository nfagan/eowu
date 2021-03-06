//
//  FilesystemWrapper.hpp
//  eowu-common
//
//  Created by Nick Fagan on 10/1/18.
//

#pragma once

#include <string>
#include <eowu-common/fs.hpp>

struct lua_State;

namespace eowu {
  class FilesystemWrapper;
}

class eowu::FilesystemWrapper {
public:
  FilesystemWrapper() = delete;
  ~FilesystemWrapper() = delete;
  
  static void CreateLuaSchema(lua_State *L);
  
private:
  static std::string get_eowu_root_directory();
  static void set_eowu_root_directory(const std::string &path);
  static std::string full_file(lua_State *L);
  static int dir(lua_State *L);
  static int find(lua_State *L);
  
  static void create_finfo_table(lua_State *L, const eowu::fs::finfo &info);
};
