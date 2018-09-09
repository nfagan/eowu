//
//  MathWrapper.hpp
//  eowu-common
//
//  Created by Nick Fagan on 9/9/18.
//

#pragma once

struct lua_State;

namespace eowu {
  class MathWrapper;
}

class eowu::MathWrapper {
public:
  MathWrapper() = delete;
  ~MathWrapper() = delete;
  
  static bool isnan(double value);
  static double nan();
  static void CreateLuaSchema(lua_State *L);
};
