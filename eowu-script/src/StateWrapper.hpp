//
//  StateWrapper.hpp
//  eowu-script
//
//  Created by Nick Fagan on 9/1/18.
//

#pragma once

#include "LuaFunction.hpp"
#include "LuaContext.hpp"
#include <memory>

namespace eowu {
  class StateWrapper;
  class State;
  struct LuaStateFunctions;
}

struct eowu::LuaStateFunctions {
  eowu::LuaFunction on_entry;
  eowu::LuaFunction on_loop;
  eowu::LuaFunction on_exit;
  
  LuaStateFunctions(const eowu::LuaFunction &entry,
                    const eowu::LuaFunction &loop,
                    const eowu::LuaFunction &exit);
  
  LuaStateFunctions(const LuaStateFunctions &other);
};

class eowu::StateWrapper {
public:
  StateWrapper(eowu::State *state,
               std::shared_ptr<eowu::LuaContext> context,
               std::unique_ptr<eowu::LuaStateFunctions> state_functions);
  
  void SetDuration(int duration);
  void SetNextState(const std::string &next);
  double Ellapsed() const;
  void Exit();
  
  static void CreateLuaSchema(lua_State *L);
private:
  
  eowu::State *state;
  std::shared_ptr<eowu::LuaContext> lua_context;
  std::unique_ptr<eowu::LuaStateFunctions> state_functions;
  
  void setup_state_callbacks();
};
