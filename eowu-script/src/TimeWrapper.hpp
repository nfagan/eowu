//
//  TimeWrapper.hpp
//  eowu-common
//
//  Created by Nick Fagan on 11/13/18.
//

#pragma once

struct lua_State;

namespace eowu {
  namespace time {
    namespace wrapper {
      void CreateLuaSchema(lua_State *L);
      double now();
    }
  }
}
