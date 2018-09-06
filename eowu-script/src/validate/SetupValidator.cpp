//
//  SetupValidator.cpp
//  eowu-common
//
//  Created by Nick Fagan on 9/5/18.
//

#include "SetupValidator.hpp"
#include "validate/Util.hpp"
#include <algorithm>

#define EOWU_RESULT_EARLY_RETURN(val) \
  if (!val.success) { \
    return val; \
  }

#define EOWU_RESULT_CONTEXT_EARLY_RETURN(val) \
  if (!val.success) { \
    val.context = context; \
    return val; \
  }

//
//  make, setup
//

eowu::schema::validate::Setup eowu::validate::make_setup(const eowu::schema::Setup &schema) {
  eowu::schema::validate::Setup setup_validator;
  
  setup_validator.geometry = make_geometry(schema);
  setup_validator.stimulus = make_stimulus(schema);
  setup_validator.target = make_target(schema);
  
  return setup_validator;
}

//
//  make, target
//

eowu::schema::validate::Target eowu::validate::make_target(const eowu::schema::Setup &schema) {
  eowu::schema::validate::Target target_validator;
  
  //  target validator
  target_validator.kinds = { "Rectangle" };
  target_validator.source_ids = eowu::validate::get_keys(schema.sources.sources);
  
  return target_validator;
}

//
//  make, stimulus
//

eowu::schema::validate::Stimulus eowu::validate::make_stimulus(const eowu::schema::Setup &schema) {
  eowu::schema::validate::Stimulus stimulus_validator;
  
  const auto geom_ids = eowu::validate::get_keys(schema.geometry.builtins.mapping);
  const auto targ_ids = eowu::validate::get_keys(schema.targets.targets);
  const auto tex_ids = eowu::validate::get_keys(schema.textures.mapping);
  
  stimulus_validator.geometry_ids = geom_ids;
  stimulus_validator.target_ids = targ_ids;
  stimulus_validator.texture_ids = tex_ids;
  stimulus_validator.units = { "normalized", "pixels" };
  
  return stimulus_validator;
}

//
//  make, gometry
//

eowu::schema::validate::Geometry eowu::validate::make_geometry(const eowu::schema::Setup &schema) {
  eowu::schema::validate::Geometry geometry_validator;
  
  geometry_validator.builtin_ids = { "Rectangle", "Circle" };
  
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
  auto pad_res = eowu::validate::check_at_least_size(schema.padding, 2, "Padding");
  EOWU_RESULT_CONTEXT_EARLY_RETURN(pad_res);
  
  //  source ids
  auto source_res = eowu::validate::check_unrecognized_one_of(validation.source_ids, schema.source_id, "Source");
  EOWU_RESULT_CONTEXT_EARLY_RETURN(source_res);
  
  //  kind ids
  auto kind_res = eowu::validate::check_unrecognized_one_of(validation.kinds, schema.kind, "Kind");
  EOWU_RESULT_CONTEXT_EARLY_RETURN(kind_res);
  
  result.success = true;
  
  return result;
}

//
//  Stimulus
//

eowu::ValidationResult eowu::validate::stimuli(const eowu::schema::Stimuli &schema,
                                               const eowu::schema::validate::Stimulus &validation) {
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
  
  //  target ids
  for (const auto &id : schema.target_ids) {
    auto targ_res = eowu::validate::check_unrecognized_one_of(validation.target_ids, id, "Target");
    EOWU_RESULT_CONTEXT_EARLY_RETURN(targ_res);
  }
  
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
