//
//  SetupParser.cpp
//  eowu-common
//
//  Created by Nick Fagan on 9/2/18.
//

#include "SetupParser.hpp"
#include "ParseUtil.hpp"
#include "Lua.hpp"

eowu::parser::ParseResult<eowu::schema::Windows> eowu::parser::windows(const luabridge::LuaRef &table) {
  using namespace eowu::parser;
  using namespace eowu::schema;
  
  ParseResult<Windows> result;
  
  auto kv = get_string_map_from_table(table);
  
  if (kv.size() == 0) {
    result.message = "No windows were specified.";
    return result;
  }
  
  for (const auto &it : kv) {
    const auto &ref = it.second;
    
    auto subtable = get_string_map_from_table(ref);
    
    Window window;
    
    window.width = get_numeric_value_or<int>(subtable, "width", -1);
    window.height = get_numeric_value_or<int>(subtable, "height", -1);
    window.index = get_numeric_value_or<unsigned int>(subtable, "index", 0);
    
    result.result.windows.emplace(it.first, window);
  }
  
  result.success = true;
  
  return result;
}

eowu::parser::ParseResult<eowu::schema::Textures> eowu::parser::textures(const luabridge::LuaRef &table) {
  using namespace eowu::parser;
  using namespace eowu::schema;
  
  ParseResult<Textures> result;

  auto kv = get_string_map_from_table(table);
  
  for (const auto &it : kv) {
    const auto &file = it.first;
    const auto &ref = it.second;
    
    if (!ref.isString()) {
      result.message = "Texture file path " + file + " must refer to a string value.";
      return result;
    }
    
    result.result.mapping.emplace(file, ref.tostring());
  }
  
  result.success = true;
  
  return result;
}

eowu::parser::ParseResult<eowu::schema::Geometry> eowu::parser::geometry(const luabridge::LuaRef &table) {
  using namespace eowu::parser;
  using namespace eowu::schema;
  
  ParseResult<Geometry> result;
  
  auto kv = get_string_map_from_table(table);
  
  bool has_builtin = kv.count("Builtin") > 0;
  
  if (has_builtin) {
    auto builtin = get_string_map_from_table(kv.at("Builtin"));
    
    for (const auto &it : builtin) {
      const auto &id = it.first;
      const auto &ref = it.second;
      
      if (!ref.isString()) {
        result.message = "Built-in identifier " + id + " must refer to a string value.";
        return result;
      }
      
      result.result.builtins.mapping.emplace(id, ref.tostring());
    }
  }
  
  result.success = true;
  
  return result;
}

eowu::parser::ParseResult<eowu::schema::Stimuli> eowu::parser::stimuli(const luabridge::LuaRef &table) {
  using namespace eowu::parser;
  using namespace eowu::schema;
  
  ParseResult<Stimuli> result;
  
  auto kv = get_string_map_from_table(table);
  
  for (const auto &it : kv) {
    auto stim_res = eowu::parser::stimulus(it.second);
    
    if (!stim_res.success) {
      result.message = stim_res.message;
      return result;
    }
    
    result.result.stimuli.emplace(it.first, stim_res.result);
  }
  
  result.success = true;
  
  return result;
}

eowu::parser::ParseResult<eowu::schema::Stimulus> eowu::parser::stimulus(const luabridge::LuaRef &table) {
  using namespace eowu::parser;
  using namespace eowu::schema;
  
  ParseResult<Stimulus> result;
  
  auto &value = result.result;
  
  auto kv = get_string_map_from_table(table);
  
  try {
    //  geometry is required
    value.geometry_id = get_string_or_error(kv, "geometry");
    value.texture_id = get_string_or_type_error(kv, "texture", "");
    value.units = get_string_or_type_error(kv, "units", "");
    value.size = get_numeric_vector_or_type_error<double>(kv, "size", value.size);
    value.position = get_numeric_vector_or_type_error<double>(kv, "position", value.position);
    
    if (kv.count("targets") > 0) {
      value.target_ids = get_string_vector_from_table(kv.at("targets"));
    }
    
  } catch (const std::exception &e) {
    result.message = e.what();
    return result;
  }
  
  result.success = true;
  
  return result;
}

eowu::parser::ParseResult<eowu::schema::Target> eowu::parser::target(const luabridge::LuaRef &table) {
  using namespace eowu::parser;
  using namespace eowu::schema;
  
  ParseResult<Target> result;
  
  auto kv = get_string_map_from_table(table);
  
  try {
    result.result.kind = get_string_or_error(kv, "kind");
    result.result.source_id = get_string_or_error(kv, "source");
    result.result.padding = get_numeric_vector_or_type_error<double>(kv, "padding", result.result.padding);
  } catch (const std::exception &e) {
    result.message = e.what();
    
    return result;
  }
  
  result.success = true;
  
  return result;
}

eowu::parser::ParseResult<eowu::schema::Targets> eowu::parser::targets(const luabridge::LuaRef &table) {
  using namespace eowu::parser;
  using namespace eowu::schema;
  
  ParseResult<Targets> result;
  
  auto kv = get_string_map_from_table(table);
  
  for (const auto &it : kv) {
    auto targ_res = eowu::parser::target(it.second);
    
    if (!targ_res.success) {
      result.message = targ_res.message;
      return result;
    }
    
    result.result.targets.emplace(it.first, targ_res.result);
  }
  
  result.success = true;
  
  return result;
}


//
//  State
//

eowu::parser::ParseResult<eowu::schema::States> eowu::parser::states(const luabridge::LuaRef &table) {
  using namespace eowu::parser;
  using namespace eowu::schema;
  
  ParseResult<States> result;
  
  auto kv = get_string_map_from_table(table);
  
  for (const auto &it : kv) {
    auto state_res = eowu::parser::state(it.second);
    
    if (!state_res.success) {
      result.message = state_res.message;
      return result;
    }
    
    result.result.mapping.emplace(it.first, state_res.result);
  }
  
  result.success = true;
  
  return result;
}

eowu::parser::ParseResult<eowu::schema::State> eowu::parser::state(const luabridge::LuaRef &table) {
  using namespace eowu::parser;
  using namespace eowu::schema;
  
  ParseResult<State> result(table.state());
  
  auto kv = get_string_map_from_table(table);
  
  try {
    result.result.duration = eowu::parser::get_numeric_value_or<double>(kv, "Duration", 0.0);
    result.result.entry_function = eowu::parser::get_function_or_error(kv, "Entry");
    result.result.loop_function = eowu::parser::get_function_or_error(kv, "Loop");
    result.result.exit_function = eowu::parser::get_function_or_error(kv, "Exit");
    result.result.render_function = eowu::parser::get_function_or_error(kv, "Render");
    
  } catch (const std::exception &e) {
    result.message = e.what();
    
    return result;
  }
  
  result.success = true;
  
  return result;
}


