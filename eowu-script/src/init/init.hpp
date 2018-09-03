//
//  init.hpp
//  eowu-script
//
//  Created by Nick Fagan on 9/3/18.
//

#pragma once

#include <memory>
#include <unordered_map>

namespace luabridge {
  class LuaRef;
}

namespace eowu {
  
  class LuaContext;
  class StateWrapper;
  class StateManager;
  using StateWrapperMapType = std::unordered_map<std::string, std::shared_ptr<eowu::StateWrapper>>;
  using StateWrapperContainerType = std::unique_ptr<eowu::StateWrapperMapType>;
  
  namespace schema {
    struct States;
  }
  
  namespace init {
    eowu::StateWrapperContainerType get_states(const eowu::schema::States &schema,
                                               std::shared_ptr<eowu::LuaContext> lua_context,
                                               eowu::StateManager &state_manager);
    
    void init_states(std::shared_ptr<eowu::LuaContext> lua_context,
                     eowu::StateManager &state_manager,
                     const eowu::schema::States &state_schema);
  }
}
