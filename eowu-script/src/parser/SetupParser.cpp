//
//  SetupParser.cpp
//  eowu-common
//
//  Created by Nick Fagan on 9/2/18.
//

#include "SetupParser.hpp"
#include "ParseUtil.hpp"
#include "Lua.hpp"

namespace util {
  std::string get_setup_schema_missing_field_error(const std::string &key) {
    return "Setup schema is missing required key: '" + key + "'";
  }
}

#define EOWU_PARSER_EARLY_RETURN(id, id_value, id_ref, param_id, function) \
  const char* const id = param_id; \
\
  if (kv.count(id) == 0) { \
    result.message = util::get_setup_schema_missing_field_error(id); \
    result.context = param_id; \
    return result; \
  } \
\
  const luabridge::LuaRef &id_ref = kv.at(id); \
  const auto id_value = function(id_ref); \
 \
  if (!id_value.success) { \
    result.message = id_value.message; \
    result.context = id_value.context; \
    return result; \
  } \
  result.result.id = id_value.result;

eowu::parser::ParseResult<eowu::schema::Setup> eowu::parser::setup(const luabridge::LuaRef &table) {
  
  using namespace eowu::parser;
  using namespace eowu::schema;
  
  ParseResult<Setup> result;
  
  auto kv = get_string_map_from_table(table);
  
  //  Windows
  EOWU_PARSER_EARLY_RETURN(windows, windows_, wref, "Windows", eowu::parser::windows);
  //  Geometry
  EOWU_PARSER_EARLY_RETURN(geometry, geometry_, gref, "Geometry", eowu::parser::geometry);
  //  Textures
  EOWU_PARSER_EARLY_RETURN(textures, textures_, tref, "Textures", eowu::parser::textures);
  //  Stimuli
  EOWU_PARSER_EARLY_RETURN(stimuli, stim_, sref, "Stimuli", eowu::parser::stimuli);
  //  Target
  EOWU_PARSER_EARLY_RETURN(targets, targets_, targref, "Targets", eowu::parser::targets);
  //  States
  EOWU_PARSER_EARLY_RETURN(states, states_, ssref, "States", eowu::parser::states);
  //  Sources
  EOWU_PARSER_EARLY_RETURN(sources, sources_, source_ref, "Sources", eowu::parser::sources);
  
  result.success = true;
  
  return result;  
}

//
//  windows
//

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
    const auto &id = it.first;
    const auto &ref = it.second;
    
    auto subtable = get_string_map_from_table(ref);
    
    Window window;
    
    window.width = get_numeric_value_or<int>(subtable, "width", -1);
    window.height = get_numeric_value_or<int>(subtable, "height", -1);
    window.index = get_numeric_value_or<unsigned int>(subtable, "index", 0);
    window.full_screen = get_numeric_value_or<unsigned int>(subtable, "fullscreen", 0);
    window.title = get_string_or_type_error(subtable, "title", id);
    
    result.result.windows.emplace(it.first, window);
  }
  
  result.success = true;
  
  return result;
}

//
//  textures
//

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
      result.context = "Textures";
      return result;
    }
    
    result.result.mapping.emplace(file, ref.tostring());
  }
  
  result.success = true;
  
  return result;
}

//
//  geometry
//

eowu::parser::ParseResult<eowu::schema::Geometry> eowu::parser::geometry(const luabridge::LuaRef &table) {
  using namespace eowu::parser;
  using namespace eowu::schema;
  
  ParseResult<Geometry> result;
  
  auto kv = get_string_map_from_table(table);
  
  bool has_builtin = kv.count("Builtin") > 0;
  
  if (has_builtin) {
    const luabridge::LuaRef &builtin_table = kv.at("Builtin");
    
    auto builtin = get_string_map_from_table(builtin_table);
    
    for (const auto &it : builtin) {
      const auto &id = it.first;
      const luabridge::LuaRef &ref = it.second;
      
      if (!ref.isString()) {
        result.message = "Built-in identifier " + id + " must refer to a string value.";
        result.context = "Geometry::Builtin";
        return result;
      }
      
      std::string val = ref.tostring();
      
      result.result.builtins.mapping.emplace(id, val);
    }
  }
  
  result.success = true;
  
  return result;
}

//
//  stimuli
//

eowu::parser::ParseResult<eowu::schema::Stimuli> eowu::parser::stimuli(const luabridge::LuaRef &table) {
  using namespace eowu::parser;
  using namespace eowu::schema;
  
  ParseResult<Stimuli> result;
  
  auto kv = get_string_map_from_table(table);
  
  for (const auto &it : kv) {
    auto stim_res = eowu::parser::stimulus(it.second, it.first);
    
    if (!stim_res.success) {
      result.message = stim_res.message;
      result.context = stim_res.context;
      return result;
    }
    
    result.result.stimuli.emplace(it.first, stim_res.result);
  }
  
  result.success = true;
  
  return result;
}

//
//  stimulus
//

eowu::parser::ParseResult<eowu::schema::Stimulus> eowu::parser::stimulus(const luabridge::LuaRef &table,
                                                                         const std::string &stimulus_id) {
  using namespace eowu::parser;
  using namespace eowu::schema;
  
  ParseResult<Stimulus> result;
  
  auto &value = result.result;
  
  auto kv = get_string_map_from_table(table);
  
  try {
    //  geometry is required
    value.stimulus_id = stimulus_id;
    value.geometry_id = get_string_or_error(kv, "geometry");
    value.texture_id = get_string_or_type_error(kv, "texture", "");
    value.units = get_string_or_type_error(kv, "units", "normalized");
    value.size = get_numeric_vector_or_type_error<double>(kv, "size", value.size);
    value.position = get_numeric_vector_or_type_error<double>(kv, "position", value.position);
    value.rotation = get_numeric_vector_or_type_error<double>(kv, "rotation", value.rotation);
    
    if (kv.count("targets") > 0) {
      value.target_ids = get_string_vector_from_table(kv.at("targets"));
    }
    
    value.provided_texture_id = value.texture_id != "";
    
  } catch (const std::exception &e) {
    result.message = e.what();
    result.context = "Stimuli::" + stimulus_id;
    return result;
  }
  
  result.success = true;
  
  return result;
}

//
//  source
//

eowu::parser::ParseResult<eowu::schema::Sources> eowu::parser::sources(const luabridge::LuaRef &table) {
  using namespace eowu::parser;
  using namespace eowu::schema;
  
  ParseResult<Sources> result;
  
  auto kv = get_string_map_from_table(table);
  
  for (const auto &it : kv) {
    auto source_res = eowu::parser::source(it.second, it.first);
    
    if (!source_res.success) {
      result.message = source_res.message;
      result.context = source_res.context;
      return result;
    }
    
    result.result.sources.emplace(it.first, source_res.result);
  }
  
  result.success = true;
  
  return result;
}

eowu::parser::ParseResult<eowu::schema::Source> eowu::parser::source(const luabridge::LuaRef &table,
                                                                     const std::string &source_id) {
  using namespace eowu::parser;
  using namespace eowu::schema;
  
  ParseResult<Source> result;
  
  auto kv = get_string_map_from_table(table);
  
  try {
    result.result.source_id = source_id;
    result.result.type = get_string_or_error(kv, "type");
  } catch (const std::exception &e) {
    result.message = e.what();
    result.context = "Sources::" + source_id;
    
    return result;
  }
  
  result.success = true;
  
  return result;
}

//
//  target
//

eowu::parser::ParseResult<eowu::schema::Target> eowu::parser::target(const luabridge::LuaRef &table,
                                                                     const std::string &target_id) {
  using namespace eowu::parser;
  using namespace eowu::schema;
  
  ParseResult<Target> result;
  
  auto kv = get_string_map_from_table(table);
  
  try {
    result.result.target_id = target_id;
    result.result.kind = get_string_or_error(kv, "kind");
    result.result.source_id = get_string_or_error(kv, "source");
    result.result.padding = get_numeric_vector_or_type_error<double>(kv, "padding", result.result.padding);
  } catch (const std::exception &e) {
    result.message = e.what();
    result.context = "Targets::" + target_id;
    
    return result;
  }
  
  result.success = true;
  
  return result;
}

//
//  targets
//

eowu::parser::ParseResult<eowu::schema::Targets> eowu::parser::targets(const luabridge::LuaRef &table) {
  using namespace eowu::parser;
  using namespace eowu::schema;
  
  ParseResult<Targets> result;
  
  auto kv = get_string_map_from_table(table);
  
  for (const auto &it : kv) {
    auto targ_res = eowu::parser::target(it.second, it.first);
    
    if (!targ_res.success) {
      result.message = targ_res.message;
      result.context = targ_res.context;
      return result;
    }
    
    result.result.targets.emplace(it.first, targ_res.result);
  }
  
  result.success = true;
  
  return result;
}


//
//  states
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
      result.context = state_res.context;
      return result;
    }
    
    result.result.mapping.emplace(it.first, state_res.result);
  }
  
  result.success = true;
  
  return result;
}

//
//  state
//

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
    result.result.state_id = eowu::parser::get_string_or_error(kv, "ID");
    result.result.is_first = eowu::parser::get_numeric_value_or<int>(kv, "First", 0);
    
    if (kv.count("Render") > 0) {
      const luabridge::LuaRef &render_function_refs = kv.at("Render");
      
      if (!render_function_refs.isTable()) {
        result.message = eowu::parser::get_type_error_message("Render", "table");
        result.context = "Render";
        return result;
      }
      
      auto render_func_result = eowu::parser::render_functions(render_function_refs);
      
      if (!render_func_result.success) {
        result.message = render_func_result.message;
        result.context = "Render";
        
        return result;
      }
      
      result.result.render_functions = render_func_result.result;
    }
    
  } catch (const std::exception &e) {
    result.message = e.what();
    result.context = "States::" + result.result.state_id;
    
    return result;
  }
  
  result.success = true;
  
  return result;
}

//
//  render functions
//

eowu::parser::ParseResult<eowu::schema::RenderFunctionAggregateType> eowu::parser::render_functions(const luabridge::LuaRef &table) {
  
  using namespace eowu::parser;
  using namespace eowu::schema;
  
  ParseResult<RenderFunctionAggregateType> result;
  
  auto kv = get_string_map_from_table(table);
  
  for (const auto &it : kv) {
    try {
      const auto &func = it.second;
      
      if (!func.isFunction()) {
        result.message = eowu::parser::get_type_error_message(it.first, "function");
        result.context = "Render";
        return result;
      }
      
      result.result.emplace(it.first, func);
      
    } catch (const std::exception &e) {
      result.message = e.what();
      result.context = "Render";
      
      return result;
    }
  }
  
  result.success = true;
  
  return result;
}

