//
//  test-noop-insert.cpp
//  eowu-common
//
//  Created by Nick Fagan on 9/6/18.
//

#include "test-noop-insert.hpp"
#include "test-util.hpp"
#include <eowu-script/eowu-script.hpp>
#include <eowu-common/test.hpp>

void eowu::test::test_noop_insert() {
  EOWU_PRINT_TEST_BEGIN("no op insert");
  
  lua_State *L = luaL_newstate();
  luaL_openlibs(L);
  
  auto lua_func = eowu::LuaFunction::get_no_op(L);
  
  EOWU_ASSERT_NOTHROW(lua_func.Call(), "Calling global no-op succeeded", "Calling global-noop threw error: ");
  
    
}
