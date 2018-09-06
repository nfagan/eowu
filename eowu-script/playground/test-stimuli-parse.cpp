//
//  test-stimuli-parse.cpp
//  eowu-script
//
//  Created by Nick Fagan on 9/2/18.
//

#include "test-stimuli-parse.hpp"
#include <eowu-script/eowu-script.hpp>
#include "Lua.hpp"
#include "test-util.hpp"

void test_stimuli() {
  
  using namespace luabridge;
  
  lua_State* L = luaL_newstate();
  luaL_openlibs(L);
  
  luabridge::LuaRef valid_setup(L);
  
  auto dir = util::get_lua_test_script_directory();
  auto file = dir + "test-parser-setup.lua";
  
  try {
    valid_setup = util::get_global_from_script_with_trap(L, file, "Setup");
  } catch (const std::runtime_error &e) {
    std::cout << "Failed to get global with message: " << e.what() << std::endl;
    return;
  }
  
  LuaRef stimuli = valid_setup["Stimuli"];
  
  if (stimuli.isNil()) {
    std::cout << "ERROR: Missing stimuli field." << std::endl;
    return;
  }
  
  auto res = eowu::parser::stimuli(stimuli);
  
  if (!res.success) {
    std::cout << "ERROR: Failed to parse valid stimuli with message: " << res.message << std::endl;
    return;
  }
  
  bool err = false;
  
  if (res.result.stimuli.count("fixation_square") == 0) {
    std::cout << "ERROR: Stimuli are missing fixation square." << std::endl;
    return;
  } else {
    std::cout << "OK: Fixation square is present" << std::endl;
  }
  
  const auto &stim = res.result.stimuli.at("fixation_square");
  
  if (stim.geometry_id != "circ") {
    std::cout << "ERROR: Stimulus does not have 'circ' geometry_id; was '" + stim.geometry_id + "'" << std::endl;
    err = true;
  }
  
  if (stim.units != "normalized") {
    std::cout << "ERROR: Stimulus does not have 'normalized' units; was '" + stim.units + "'" << std::endl;
    err = true;
  }
  
  const auto &sz = stim.size;
  
  if (sz.size() != 2 || sz[0] != 1.0) {
    std::cout << "ERROR: Stimulus size was not parsed correct." << std::endl;
    err = true;
  }
  
  if (!err) {
    std::cout << "OK: valid stimulus parsed successfully" << std::endl;
  }
}

void test_stimuli_run_all() {
  std::cout << "--------" << "stimuli-parser" << std::endl;
  
  test_stimuli();
}
