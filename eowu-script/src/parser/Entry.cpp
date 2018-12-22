//
//  Entry.cpp
//  eowu-common
//
//  Created by Nick Fagan on 9/5/18.
//

#include "SetupParser.hpp"
#include "ParseUtil.hpp"
#include "Lua.hpp"
#include "../init/init.hpp"
#include <eowu-common/fs.hpp>
#include <eowu-common/platform.hpp>
#include <stdexcept>

eowu::parser::ParseResult<bool> eowu::parser::insert_package_path(lua_State *L, const std::string &file) {
  eowu::parser::ParseResult<bool> result;
  
  result.result = false;
  result.context = "parser::insert_package_path";
  
  auto outer_dir = eowu::fs::get_outer_directory(file);
  
  //  if we start the utility with a single file path like: `eowu setup.lua`
  //  then there won't be an outer directory.
  if (outer_dir == "") {
    result.success = true;
    return result;
  }
  
  std::string package_executor = "package.path = package.path .. (';' .. '" +  outer_dir + "' .. '/?.lua')";
  const char* const buff = package_executor.c_str();
  
  int error = luaL_loadbuffer(L, buff, strlen(buff), "line") || lua_pcall(L, 0, 0, 0);
  
  if (error != 0) {
    std::string lua_error_message = lua_tostring(L, -1);
    std::string base_error = "Failed to insert package directory: '" + outer_dir + "' into file: '" + file;
    result.message = base_error + "' with message: " + lua_error_message;
    
    return result;
  }
  
  result.success = true;
  result.result = true;
  
  return result;
}

eowu::parser::ParseResult<eowu::schema::EntryScript> eowu::parser::entry_script(lua_State *L, const std::string &file) {
  //  result
  eowu::parser::ParseResult<eowu::schema::EntryScript> entry_script(L);
  int lua_status;

#ifdef EOWU_IS_MAC
  //  insert package path into the executing file
  auto insert_result = parser::insert_package_path(L, file);

  if (!insert_result.success) {
    entry_script.message = insert_result.message;
    entry_script.context = insert_result.context;
    return entry_script;
  }
#endif
  
  lua_status = luaL_loadfile(L, file.c_str());
  
  //  error parsing file
  if (lua_status != 0) {
    std::string lua_error_message = lua_tostring(L, -1);
    
    entry_script.message = "Parsing entry script failed with message: " + lua_error_message;
    
    return entry_script;
  }
 
  //  attempt to run the setup file
  try {
    lua_status = lua_pcall(L, 0, 0, 0);
    
  } catch (const std::exception &e) {
    entry_script.message = std::string("Loading entry script failed with message: ") + e.what();
    
    return entry_script;
  }
  
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
