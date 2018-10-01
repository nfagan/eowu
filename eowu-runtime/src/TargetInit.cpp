//
//  TargetInit.cpp
//  eowu-common
//
//  Created by Nick Fagan on 9/15/18.
//

#include "TargetInit.hpp"
#include "Util.hpp"
#include <eowu-gl/eowu-gl.hpp>
#include <eowu-script/eowu-script.hpp>
#include <eowu-common/random.hpp>
#include <cstddef>

#define EOWU_TARGET_RESULT_EARLY_RETURN(from, to) \
  if (!from.status.success) { \
    to.status.message = from.status.message; \
    to.status.context = from.status.context; \
    return to; \
  }

eowu::init::TargetResult eowu::init::initialize_targets(const eowu::schema::Targets &schema,
                                                        const eowu::init::XYSourceMapType &xy_sources,
                                                        std::shared_ptr<eowu::LuaContext> lua_context,
                                                        std::shared_ptr<eowu::GLPipeline> gl_pipeline) {
  eowu::init::TargetResult result;
  
  result.status.context = "init::initialize_targets";
  
  const auto resource_manager = gl_pipeline->GetResourceManager();
  const auto context_manager = gl_pipeline->GetContextManager();
  const auto renderer = gl_pipeline->GetRenderer();
  const auto window_container = gl_pipeline->GetWindowContainer();
  
  std::unordered_map<std::string, std::shared_ptr<eowu::Model>> target_models_by_type;
  auto mesh_factory_map = eowu::init::get_target_type_to_mesh_factory_map();
  
  for (const auto &it : schema.targets) {
    const auto &target_schema = it.second;
    const auto &source_id = target_schema.source_id;
    const auto &target_id = target_schema.target_id;
    const auto &target_type = target_schema.type;
    const auto &model_id = target_schema.model_id;
    const auto &window_id = target_schema.window_id;
    const bool provided_model_id = target_schema.provided_model_id;
    const bool is_hidden = target_schema.is_hidden;
    
    auto target = std::make_shared<eowu::XYTarget>();
    
    //  shouldn't happen (source id validation should already be taken care of)
    //  but not bad to check.
    if (xy_sources.count(source_id) == 0) {
      result.status.message = "No xy-source matched id '" + source_id + "'.";
      return result;
    }
    
    //  also shouldn't happen (model id validation should already be taken care of)
    if (provided_model_id) {
      std::shared_ptr<eowu::Model> linked_model = nullptr;
      
      try {
        linked_model = resource_manager->Get<eowu::Model>(model_id);
      } catch (const std::exception &e) {
        result.status.message = e.what();
        return result;
      }
      
      target->LinkModel(linked_model);
    }
    
    //  bounds function
    if (target_type == "Circle") {
      target->SetBoundsFunction(eowu::bounds_functions::circle_in_bounds);
    } else if (target_type == "Rectangle") {
      target->SetBoundsFunction(eowu::bounds_functions::rect_in_bounds);
    } else {
      result.status.message = "Unimplemented target type: '" + target_type + "'.";
      return result;
    }
    
    std::shared_ptr<eowu::Model> target_model = nullptr;
    
    //  target model
    if (target_models_by_type.count(target_type) == 0) {
      const std::string id = eowu::init::get_target_id(resource_manager);
      
      auto mesh = resource_manager->Create<eowu::Mesh>(id);
      auto mat = resource_manager->Create<eowu::Material>(id);
      
      const auto &mesh_func_it = mesh_factory_map.find(target_type);
      
      if (mesh_func_it == mesh_factory_map.end()) {
        result.status.message = "Target type '" + target_type + "' has no associated model.";
        return result;
      }
      
      //  white frame by default.
      mat->SetFaceColor(glm::vec3(1.0));
      
      //  create the mesh.
      mesh_func_it->second(mesh.get());
      
      target_model = resource_manager->Create<eowu::Model>(id, mesh, mat);
      target_models_by_type.emplace(target_type, target_model);
    } else {
      target_model = target_models_by_type.at(target_type);
    }
    
    //  window
    std::shared_ptr<eowu::Window> win;
    
    try {
      win = context_manager->GetWindowByAlias(window_id);
    } catch (const std::exception &e) {
      result.status.message = e.what();
      return result;
    }
    
    const eowu::XYSource *source = xy_sources.at(source_id);
    const glm::vec2 padding = eowu::util::require_vec2(target_schema.padding);
    
    target->SetSource(source);
    target->SetPadding(padding);
    target->SetWindow(win.get());
    target->SetAlias(target_id);
    
    result.result.targets.emplace(target_id, target);
    
    //  make wrapper
    auto target_wrapper = std::make_shared<eowu::TargetWrapper>(lua_context, target,
                                                                target_model, renderer, window_container);
    
    if (is_hidden) {
      target_wrapper->Hide();
    }
    
    result.result.target_wrappers.emplace(target_id, target_wrapper);
    
  }
  
  result.status.success = true;
  
  return result;
}

std::unordered_map<std::string, eowu::init::MeshFactoryFunction> eowu::init::get_target_type_to_mesh_factory_map() {
  return {
    {"Circle", &eowu::mesh_factory::make_frame_circle},
    {"Rectangle", &eowu::mesh_factory::make_frame_quad}
  };
}

std::string eowu::init::get_target_id(const std::shared_ptr<eowu::ResourceManager> &resource_manager) {
  std::string id = eowu::random::get_random_string(20);
  
  while (resource_manager->Has<eowu::Model>(id)) {
    id = eowu::random::get_random_string(20);
  }
  
  return id;
}
