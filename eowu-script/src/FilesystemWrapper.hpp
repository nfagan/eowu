//
//  FilesystemWrapper.hpp
//  eowu-common
//
//  Created by Nick Fagan on 10/1/18.
//

#pragma once

#include <string>

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
  static std::string full_file(lua_State *L);
};
