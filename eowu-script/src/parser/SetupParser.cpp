//
//  SetupParser.cpp
//  eowu-common
//
//  Created by Nick Fagan on 9/2/18.
//

#include "SetupParser.hpp"
#include "ParseUtil.hpp"
#include "Lua.hpp"
#include "../LuaFunction.hpp"
#include "../data/conversion.hpp"

namespace util {
  std::string get_setup_schema_missing_field_error(const std::string &key) {
    return "Setup schema is missing required key: '" + key + "'";
  }
}

#define EOWU_PARSER_EARLY_RETURN_ALLOW_MISSING(in, out, assign) \
  if (!in.success) { \
    out.message = in.message; \
    out.context = in.context; \
    return out; \
  } \
  out.result.assign = std::move(in.result);


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
  //  Stimuli
  EOWU_PARSER_EARLY_RETURN(stimuli, stim_, sref, "Stimuli", eowu::parser::stimuli);
  //  States
  EOWU_PARSER_EARLY_RETURN(states, states_, ssref, "States", eowu::parser::states);
  
  //  Sources
  if (kv.count("Sources") > 0) {
    auto source_result = eowu::parser::sources(kv.at("Sources"));
    EOWU_PARSER_EARLY_RETURN_ALLOW_MISSING(source_result, result, sources);
  }
  
  //  Targets
  if (kv.count("Targets") > 0) {
    auto targ_result = eowu::parser::targets(kv.at("Targets"));
    EOWU_PARSER_EARLY_RETURN_ALLOW_MISSING(targ_result, result, targets);
  }
  
  //  Textures
  if (kv.count("Textures") > 0) {
    auto tex_result = eowu::parser::textures(kv.at("Textures"));
    EOWU_PARSER_EARLY_RETURN_ALLOW_MISSING(tex_result, result, textures);
  }
  
  //  Save
  if (kv.count("Save") > 0) {
    auto save_result = eowu::parser::save(kv.at("Save"));
    EOWU_PARSER_EARLY_RETURN_ALLOW_MISSING(save_result, result, save);
  }
  
  //  Paths
  if (kv.count("Paths") > 0) {
    auto path_result = eowu::parser::paths(kv.at("Paths"));
    EOWU_PARSER_EARLY_RETURN_ALLOW_MISSING(path_result, result, paths);
  }
  
  //  Variables
  if (kv.count("Variables") > 0) {
    auto variable_result = eowu::parser::variables(kv.at("Variables"));
    EOWU_PARSER_EARLY_RETURN_ALLOW_MISSING(variable_result, result, variables);
  }
  
  result.success = true;
  
  return result;  
}

//
//  save
//

eowu::parser::ParseResult<eowu::schema::Save> eowu::parser::save(const luabridge::LuaRef &table) {
  eowu::parser::ParseResult<eowu::schema::Save> result;
  
  auto kv = get_string_map_from_table(table);
  
  try {
    result.result.save_state_data = get_numeric_value_or<bool>(kv, "states", false);
    
    if (kv.count("sources") > 0) {
      auto v_source_ids = get_string_vector_from_table(kv.at("sources"));
      
      std::unordered_set<std::string> s_source_ids(v_source_ids.begin(), v_source_ids.end());
      
      result.result.source_ids = std::move(s_source_ids);
      result.result.provided_source_ids = true;
    }
    
  } catch (const std::exception &e) {
    result.message = e.what();
    result.context = "Save";
    
    return result;
  }
  
  result.success = true;
  
  return result;
}

//
//  paths
//

eowu::parser::ParseResult<eowu::schema::Paths> eowu::parser::paths(const luabridge::LuaRef &table) {
  eowu::parser::ParseResult<eowu::schema::Paths> result;
  
  auto kv = get_string_map_from_table(table);
  
  try {
    if (kv.count("data") > 0) {
      result.result.data = get_string_or_error(kv, "data");
      result.result.provided_data = true;
    }
  } catch (const std::exception &e) {
    result.message = e.what();
    result.context = "Paths";
    
    return result;
  }
  
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
    window.is_fullscreen = get_numeric_value_or<bool>(subtable, "fullscreen", false);
    window.is_resizeable = get_numeric_value_or<bool>(subtable, "resizeable", false);
    window.is_vsynced = get_numeric_value_or<bool>(subtable, "vsync", true);
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
    value.color = get_numeric_vector_or_type_error<double>(kv, "color", value.color);
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
    
    //  has window id
    if (kv.count("window") > 0) {
      result.result.window_id = get_string_or_type_error(kv, "window", "");
      result.result.provided_window_id = true;
    }
    
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
    result.result.type = get_string_or_error(kv, "type");
    result.result.source_id = get_string_or_error(kv, "source");
    result.result.window_id = get_string_or_error(kv, "window");
    result.result.padding = get_numeric_vector_or_type_error<double>(kv, "padding", result.result.padding);
    
    if (kv.count("stimulus") > 0) {
      result.result.model_id = get_string_or_error(kv, "stimulus");
      result.result.provided_model_id = true;
    }
    
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
//  variables
//

eowu::parser::ParseResult<eowu::schema::Variables> eowu::parser::variables(const luabridge::LuaRef &table) {
  ParseResult<eowu::schema::Variables> result;
  
  try {
    result.result.mapping = eowu::parser::get_variables(table);
  } catch (const std::exception &e) {
    result.message = e.what();
    result.context = "Variables";
    
    return result;
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
  
  //  global noop function
  const auto &noop = eowu::LuaFunction::get_no_op(table.state()).GetReference();
  
  try {
    result.result.state_id = eowu::parser::get_string_or_error(kv, "Name");
    result.result.duration = eowu::parser::get_numeric_value_or<double>(kv, "Duration", 0.0);
    result.result.entry_function = eowu::parser::get_function_or_error(kv, "Entry");
    result.result.loop_function = eowu::parser::get_function_or_type_error(kv, "Loop", noop);
    result.result.exit_function = eowu::parser::get_function_or_type_error(kv, "Exit", noop);
    result.result.is_first = eowu::parser::get_numeric_value_or<bool>(kv, "First", false);
    
    if (kv.count("Variables") > 0) {
      const luabridge::LuaRef &var_ref = kv.at("Variables");
      result.result.variables = eowu::parser::get_variables(var_ref);
    }
    
    if (kv.count("Render") > 0) {
      const luabridge::LuaRef &render_function_refs = kv.at("Render");
      auto render_func_result = eowu::parser::function_aggregate(render_function_refs, "Render");
      
      if (!render_func_result.success) {
        result.message = render_func_result.message;
        return result;
      }
      
      result.result.render_functions = render_func_result.result;
    }
    
    if (kv.count("Flip") > 0) {
      const luabridge::LuaRef &flip_function_refs = kv.at("Flip");
      auto flip_func_result = eowu::parser::function_aggregate(flip_function_refs, "Flip");
      
      if (!flip_func_result.success) {
        result.message = flip_func_result.message;
        return result;
      }
      
      result.result.flip_functions = flip_func_result.result;
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

eowu::parser::ParseResult<eowu::schema::RefFunctionAggregateType> eowu::parser::function_aggregate(const luabridge::LuaRef &table,
                                                                                                   const std::string &key) {
  using namespace eowu::parser;
  using namespace eowu::schema;
  
  ParseResult<RefFunctionAggregateType> result;
  
  if (!table.isTable()) {
    result.message = eowu::parser::get_type_error_message(key, "table");
    result.context = key;
    return result;
  }
  
  auto kv = get_string_map_from_table(table);
  
  for (const auto &it : kv) {
    try {
      const auto &func = it.second;
      
      if (!func.isFunction()) {
        result.message = eowu::parser::get_type_error_message(it.first, "function");
        result.context = key;
        return result;
      }
      
      result.result.emplace(it.first, func);
      
    } catch (const std::exception &e) {
      result.message = e.what();
      result.context = key;
      
      return result;
    }
  }
  
  result.success = true;
  
  return result;
}

