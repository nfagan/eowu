//
//  Entry.cpp
//  eowu-common
//
//  Created by Nick Fagan on 9/5/18.
//

#include "SetupParser.hpp"
#include "ParseUtil.hpp"
#include "Lua.hpp"

eowu::parser::ParseResult<eowu::schema::EntryScript> eowu::parser::entry_script(lua_State *L, const std::string &file) {
  //  result
  eowu::parser::ParseResult<eowu::schema::EntryScript> entry_script(L);
  int lua_status;
  
  lua_status = luaL_loadfile(L, file.c_str());
  
  //  error parsing file
  if (lua_status != 0) {
    std::string lua_error_message = lua_tostring(L, -1);
    
    entry_script.message = "Parsing entry script failed with message: " + lua_error_message;
    
    return entry_script;
  }
  
  lua_status = lua_pcall(L, 0, 0, 0);
  
  //  error calling file
  if (lua_status != 0) {
    std::string lua_error_message = lua_tostring(L, -1);
    
    entry_script.message = "Invoking entry script failed with message: " + lua_error_message;
    
    return entry_script;
  }
  
  const std::string empty_table_message = "The file '" + file + "' must contain a table named 'Setup'.";
  
  auto entry_table = luabridge::getGlobal(L, "Setup");
  
  if (!entry_table.isTable()) {
    entry_script.message = empty_table_message;
    
    return entry_script;
  }
  
  auto kv = eowu::parser::get_string_map_from_table(entry_table);
  
  if (kv.size() == 0) {
    entry_script.message = empty_table_message;
    return entry_script;
  }
  
  entry_script.result.entry_table = entry_table;
  entry_script.success = true;
  
  return entry_script;
}
