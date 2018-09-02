//
//  StateWrapper.hpp
//  eowu-script
//
//  Created by Nick Fagan on 9/1/18.
//

#pragma once

struct lua_State;

namespace eowu {
  class StateWrapper;
  class State;
}

class eowu::StateWrapper {
public:
  StateWrapper(eowu::State *state);
  
  void SetDuration(int duration);
  
  static void CreateLuaSchema(lua_State *L);
private:
  eowu::State *state;
  
};
