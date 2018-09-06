//
//  test-state-wrapper.cpp
//  eowu-script
//
//  Created by Nick Fagan on 9/3/18.
//

#include "test-state-wrapper.hpp"
#include "test-util.hpp"
#include <eowu-script/eowu-script.hpp>
#include <eowu-state/eowu-state.hpp>
#include <Lua.hpp>
#include <unordered_map>
#include <string>
#include <memory>
#include <assert.h>
#include <stdexcept>

namespace test {
  
  void test_complete() {
    using namespace luabridge;
    using namespace eowu;
    
    StateManager state_manager;
    StateRunner runner;
    
    lua_State* L = luaL_newstate();
    luaL_openlibs(L);
    
    auto lua_context = std::make_shared<LuaContext>(L);
    
    auto file = util::get_lua_test_script_directory() + "test-state-wrapper.lua";
    LuaRef states = util::get_global_from_script_with_trap(L, file, "States");
    
    auto state_schemas = parser::states(states);
    
    if (!state_schemas.success) {
      std::cout << "ERROR: Failed to parse schema." << std::endl;
      return;
    }
    
    init::init_state_schema(lua_context->GetState());
    
    eowu::TaskWrapper::States = init::get_states(state_schemas.result, lua_context, state_manager);
    
    runner.Begin(state_manager.GetState("1"));
    
    while (!runner.Update()) {
      //
    }
  }
  
  void test_run() {
    using namespace luabridge;
    using namespace eowu;
    
    lua_State* L = luaL_newstate();
    luaL_openlibs(L);
    
    auto lua_context = std::make_shared<LuaContext>(L);
    
    StateWrapper::CreateLuaSchema(L);
    
    auto file = util::get_lua_test_script_directory() + "test-state-wrapper.lua";
    auto state1 = util::get_global_from_script_with_trap(L, file, "State1");
    auto state2 = util::get_global_from_script_with_trap(L, file, "State2");
    
    auto schema1 = parser::state(state1);
    auto schema2 = parser::state(state2);
    
    assert(schema1.success && schema2.success);
    
    StateManager manager;
    StateRunner runner;
    
    std::vector<decltype(schema1)> schemas = { schema1, schema2 };
    std::vector<State*> states;
    std::vector<std::shared_ptr<StateWrapper>> wrappers;
    
    for (const auto &schema : schemas) {      
      auto state = manager.CreateState(schema.result.state_id);
      states.push_back(state);
      
      if (schema.result.duration > 0) {
        state->SetDuration(std::chrono::seconds((int)schema.result.duration));
      }
      
      LuaReferenceContainer ref_entry(schema.result.entry_function);
      LuaReferenceContainer ref_exit(schema.result.exit_function);
      LuaReferenceContainer ref_loop(schema.result.loop_function);
      
      LuaFunction entry(ref_entry);
      LuaFunction exit(ref_exit);
      LuaFunction loop(ref_loop);
      
      auto state_functions = std::make_unique<LuaStateFunctions>(entry, loop, exit);     
      auto wrapper = std::make_shared<StateWrapper>(state, lua_context, std::move(state_functions));
      
//      state_manager.wrappers.emplace(schema.result.state_id, std::move(wrapper));
    }
    
    runner.Begin(states[0]);
    
    while (!runner.Update()) {
      //
    }
  }
}


void test::test_state_wrapper_run_all() {
  test::test_complete();
}
