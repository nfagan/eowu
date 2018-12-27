//
//  KeyboardWrapper.hpp
//  eowu-common
//
//  Created by Nick Fagan on 9/18/18.
//

#pragma once

#include <string>

struct lua_State;

namespace eowu {
  class KeyboardWrapper;
  class Keyboard;
}

class eowu::KeyboardWrapper {
  
public:
  KeyboardWrapper(eowu::Keyboard &keyboard);
  
  bool WasPressed(const std::string &key);
  bool IsDown(const std::string &key) const;
  int GetKeyNames(lua_State *L);
  
  static void CreateLuaSchema(lua_State *L);
private:
  eowu::Keyboard &keyboard;
  
  void assert_key_name(const std::string &key) const;
};
