//
//  init.hpp
//  eowu-script
//
//  Created by Nick Fagan on 9/3/18.
//

#pragma once

#include <memory>
#include <unordered_map>

struct lua_State;

namespace luabridge {
  class LuaRef;
}

namespace eowu {
  
  class LuaContext;
  class LuaFunction;
  class GLPipeline;
  class StateWrapper;
  class StateManager;
  class StateRunner;
  class State;
  
  using StateWrapperMapType = std::unordered_map<std::string, std::shared_ptr<eowu::StateWrapper>>;
  using StateWrapperContainerType = std::unique_ptr<eowu::StateWrapperMapType>;
  using LuaFunctionMapType = std::unordered_map<std::string, LuaFunction>;
  using LuaFunctionContainerType = std::unique_ptr<LuaFunctionMapType>;
  
  namespace schema {
    struct States;
  }
  
  namespace init {
    eowu::StateWrapperContainerType get_states(const eowu::schema::States &schema,
                                               std::shared_ptr<eowu::LuaContext> lua_context,
                                               eowu::StateManager &state_manager,
                                               eowu::StateRunner &state_runner);
    
    eowu::State* get_first_state(const eowu::schema::States &schema,
                                 const eowu::StateManager &state_manager);
    
    eowu::LuaFunctionContainerType get_render_functions(const eowu::schema::States &schema);
    eowu::LuaFunctionContainerType get_flip_functions(const eowu::schema::States &schema);
    
    void init_state_schema(lua_State *L);
    void init_render_schema(lua_State *L);
  }
}
