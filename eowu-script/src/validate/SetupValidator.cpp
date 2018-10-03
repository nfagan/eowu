//
//  SetupValidator.cpp
//  eowu-common
//
//  Created by Nick Fagan on 9/5/18.
//

#include "SetupValidator.hpp"
#include "validate/Util.hpp"
#include <algorithm>
#include <eowu-gl/eowu-gl.hpp>

#define EOWU_RESULT_EARLY_RETURN(val) \
  if (!val.success) { \
    return val; \
  }

#define EOWU_RESULT_CONTEXT_EARLY_RETURN(val) \
  if (!val.success) { \
    val.context = context; \
    return val; \
  }

#define EOWU_RESULT_ASSIGN_EARLY_RETURN(in, out) \
  if (!in.success) { \
    out.message = in.message; \
    out.context = in.context; \
    return out; \
  }

//
//  make, setup
//

eowu::schema::validate::Setup eowu::validate::make_setup(const eowu::schema::Setup &schema) {
  eowu::schema::validate::Setup setup_validator;
  
  setup_validator.geometry = make_geometry(schema);
  setup_validator.stimulus = make_stimulus(schema);
  setup_validator.target = make_target(schema);
  setup_validator.source = make_source(schema);
  
  return setup_validator;
}

//
//  make, source
//

eowu::schema::validate::Source eowu::validate::make_source(const eowu::schema::Setup &schema) {
  eowu::schema::validate::Source source_validator;
  
  source_validator.source_types = {"Mouse", "Keyboard"};
  source_validator.window_ids = eowu::validate::get_keys(schema.windows.windows);
  
  return source_validator;
}

//
//  make, target
//

eowu::schema::validate::Target eowu::validate::make_target(const eowu::schema::Setup &schema) {
  eowu::schema::validate::Target target_validator;
  
  //  target validator
  target_validator.kinds = {"Circle", "Rectangle"};
  target_validator.source_ids = eowu::validate::get_keys(schema.sources.sources);
  target_validator.model_ids = eowu::validate::get_keys(schema.stimuli.stimuli);
  
  return target_validator;
}

//
//  make, stimulus
//

eowu::schema::validate::Stimulus eowu::validate::make_stimulus(const eowu::schema::Setup &schema) {
  eowu::schema::validate::Stimulus stimulus_validator;
  
  const auto geom_ids = eowu::validate::get_keys(schema.geometry.builtins.mapping);
  const auto tex_ids = eowu::validate::get_keys(schema.textures.mapping);
  
  stimulus_validator.geometry_ids = geom_ids;
  stimulus_validator.texture_ids = tex_ids;
  stimulus_validator.units = eowu::units::get_string_unit_labels();
  
  return stimulus_validator;
}

//
//  make, gometry
//

eowu::schema::validate::Geometry eowu::validate::make_geometry(const eowu::schema::Setup &schema) {
  eowu::schema::validate::Geometry geometry_validator;
  
  geometry_validator.builtin_ids = {"Rectangle", "Circle", "Triangle", "RectangleFrame", "CircleFrame", "TriangleFrame"};
  
  return geometry_validator;
}

//
//  Setup
//

eowu::ValidationResult eowu::validate::setup(const eowu::schema::Setup &schema,
                                             const eowu::schema::validate::Setup &validation) {
  
  eowu::ValidationResult result;
  
  //  geometry
  auto geom_res = validate::geometry(schema.geometry, validation.geometry);
  EOWU_RESULT_EARLY_RETURN(geom_res);
  
  //  targets
  auto targ_res = validate::targets(schema.targets, validation.target);
  EOWU_RESULT_EARLY_RETURN(targ_res);
  
  //  stimuli
  auto stim_res = validate::stimuli(schema.stimuli, validation.stimulus);
  EOWU_RESULT_EARLY_RETURN(stim_res);
  
  //  states
  auto state_res = validate::states(schema.states);
  EOWU_RESULT_EARLY_RETURN(state_res);
  
  //  sources
  auto sources_res = validate::sources(schema.sources, validation.source);
  EOWU_RESULT_EARLY_RETURN(sources_res);
  
  //  io
  auto io_res = validate::io(schema.paths, schema.save);
  EOWU_RESULT_EARLY_RETURN(io_res);
  
  result.success = true;
  
  return result;
}

//
//  io
//

eowu::ValidationResult eowu::validate::io(const eowu::schema::Paths &paths_schema, const eowu::schema::Save &save_schema) {
  eowu::ValidationResult result;
  
  bool any_data_to_save = save_schema.source_ids.size() > 0 || save_schema.save_state_data;
  
  //  ensure that, if we don't provide a 'data' path, then we didn't specify any
  //  data to be saved.
  if (any_data_to_save && !paths_schema.provided_data) {
    result.message = "Requested that state or source data be saved, but no 'data' path was specified.";
    result.context = "Paths::data";
    
    return result;
  }
  
  result.success = true;
  
  return result;
}

//
//  State
//

eowu::ValidationResult eowu::validate::states(const eowu::schema::States &schema) {
  eowu::ValidationResult result;
  
  std::unordered_set<std::string> visited_render_functions;
  std::unordered_set<std::string> visited_flip_functions;
  std::unordered_set<std::string> visited_state_ids;
  
  //  ensure render functions + flip functions don't contain duplicates
  for (const auto &it : schema.mapping) {
    const auto &state = it.second;
    const auto &render_functions = state.render_functions;
    const auto &flip_functions = state.flip_functions;
    
    const auto context = "State::" + state.state_id;
    
    auto render_res = map_check_duplicates(visited_render_functions, render_functions, "render function", context);
    auto flip_res = map_check_duplicates(visited_flip_functions, flip_functions, "flip function", context);
    
    EOWU_RESULT_ASSIGN_EARLY_RETURN(render_res, result);
    EOWU_RESULT_ASSIGN_EARLY_RETURN(flip_res, result);
  }
  
  //  ensure state ids are unique
  for (const auto &it : schema.mapping) {
    const auto &state = it.second;
    const auto &state_id = state.state_id;
    
    const auto context = "State::" + state.state_id;
    
    if (visited_state_ids.count(state_id) > 0) {
      result.message = "Duplicate state id: '" + state_id + "'.";
      result.context = context;
      
      return result;
    }
    
    visited_state_ids.emplace(state_id);
  }
  
  result.success = true;
  
  return result;
}

//
//  Source
//

eowu::ValidationResult eowu::validate::sources(const eowu::schema::Sources &schema,
                                               const eowu::schema::validate::Source &validation) {
  eowu::ValidationResult result;
  
  for (const auto &it : schema.sources) {
    auto res = eowu::validate::source(it.second, validation);
    
    if (!res.success) {
      return res;
    }
  }
  
  result.success = true;
  
  return result;
  
}

eowu::ValidationResult eowu::validate::source(const eowu::schema::Source &schema,
                                              const eowu::schema::validate::Source &validation) {
  eowu::ValidationResult result;
  
  const auto context = "Sources::" + schema.source_id;
  
  //  source types
  auto type_res = eowu::validate::check_unrecognized_one_of(validation.source_types, schema.type, "type");
  EOWU_RESULT_CONTEXT_EARLY_RETURN(type_res);
  
  if (schema.type == "Mouse") {
    if (!schema.provided_window_id) {
      result.message = "Source type: '" + schema.type + "' is missing required key: 'window'.";
      result.context = context;
      return result;
    }
  }
  
  //  window ids
  if (schema.type == "Mouse") {
    auto win_res = eowu::validate::check_unrecognized_one_of(validation.window_ids, schema.window_id, "Window");
    EOWU_RESULT_CONTEXT_EARLY_RETURN(win_res);
  }
  
  result.success = true;
  
  return result;
}

//
//  Target
//

eowu::ValidationResult eowu::validate::targets(const eowu::schema::Targets &schema,
                                               const eowu::schema::validate::Target &validation) {
  eowu::ValidationResult result;
  
  for (const auto &it : schema.targets) {
    auto res = eowu::validate::target(it.second, validation);
    
    if (!res.success) {
      return res;
    }
  }
  
  result.success = true;
  
  return result;
}

eowu::ValidationResult eowu::validate::target(const eowu::schema::Target &schema,
                                              const eowu::schema::validate::Target &validation) {
  
  eowu::ValidationResult result;
  
  const auto context = "Targets::" + schema.target_id;
  
  //  padding
  auto pad_res = eowu::validate::check_at_least_size(schema.padding, 2, "padding");
  EOWU_RESULT_CONTEXT_EARLY_RETURN(pad_res);
  
  //  source ids
  auto source_res = eowu::validate::check_unrecognized_one_of(validation.source_ids, schema.source_id, "source");
  EOWU_RESULT_CONTEXT_EARLY_RETURN(source_res);
  
  //  kind ids
  auto kind_res = eowu::validate::check_unrecognized_one_of(validation.kinds, schema.type, "type");
  EOWU_RESULT_CONTEXT_EARLY_RETURN(kind_res);
  
  //  model ids
  if (schema.provided_model_id) {
    auto model_res = eowu::validate::check_unrecognized_one_of(validation.model_ids, schema.model_id, "stimulus");
    EOWU_RESULT_CONTEXT_EARLY_RETURN(model_res);
  }
  
  result.success = true;
  
  return result;
}

//
//  Stimulus
//

eowu::ValidationResult eowu::validate::stimuli(const eowu::schema::Stimuli &schema,
                                               const eowu::schema::validate::Stimulus &validation) {
  
  //  @TODO: Ensure target ids are unique across stimuli.
  eowu::ValidationResult result;
  
  for (const auto &it : schema.stimuli) {
    auto res = eowu::validate::stimulus(it.second, validation);
    
    if (!res.success) {
      return res;
    }
  }
  
  result.success = true;
  
  return result;
}

eowu::ValidationResult eowu::validate::stimulus(const eowu::schema::Stimulus &schema,
                                                const eowu::schema::validate::Stimulus &validation) {
  eowu::ValidationResult result;
  
  const auto context = "Stimuli::" + schema.stimulus_id;
  
  //  texture ids
  eowu::ValidationResult tex_res;
  
  if (schema.provided_texture_id) {
    tex_res = eowu::validate::check_unrecognized_one_of(validation.texture_ids, schema.texture_id, "Texture");
  } else {
    tex_res.success = true;
  }
  
  EOWU_RESULT_CONTEXT_EARLY_RETURN(tex_res);
  
  //  geometry ids
  auto geom_res = eowu::validate::check_unrecognized_one_of(validation.geometry_ids, schema.geometry_id, "Geometry");
  EOWU_RESULT_CONTEXT_EARLY_RETURN(geom_res);
  
  //  units
  auto unit_res = eowu::validate::check_unrecognized_one_of(validation.units, schema.units, "Units");
  EOWU_RESULT_CONTEXT_EARLY_RETURN(unit_res);
  
  //  size
  auto size_res = eowu::validate::check_at_least_size(schema.size, 2, "Size");
  EOWU_RESULT_CONTEXT_EARLY_RETURN(size_res);
  
  //  position
  auto pos_res = eowu::validate::check_at_least_size(schema.position, 2, "Position");
  EOWU_RESULT_CONTEXT_EARLY_RETURN(pos_res);
  
  result.success = true;
  
  return result;
}

//
//  Geometry
//

eowu::ValidationResult eowu::validate::geometry(const eowu::schema::Geometry &schema,
                                                const eowu::schema::validate::Geometry &validator) {
  
  eowu::ValidationResult result;
  
  const auto context = "Geometry::Builtin";
  
  const auto &builtins = validator.builtin_ids;
  
  for (const auto &it : schema.builtins.mapping) {
    auto geom_res = eowu::validate::check_unrecognized_one_of(builtins, it.second, "Geometry");
    EOWU_RESULT_CONTEXT_EARLY_RETURN(geom_res);
  }
  
  result.success = true;
  
  return result;
}
