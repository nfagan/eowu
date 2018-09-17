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

#define EOWU_TARGET_RESULT_EARLY_RETURN(from, to) \
  if (!from.status.success) { \
    to.status.message = from.status.message; \
    to.status.context = from.status.context; \
    return to; \
  }

eowu::init::TargetResult eowu::init::initialize_targets(const eowu::schema::Targets &schema,
                                                        const eowu::init::XYSourceMapType &xy_sources,
                                                        std::shared_ptr<eowu::GLPipeline> gl_pipeline) {
  eowu::init::TargetResult result;
  
  result.status.context = "init::initialize_targets";
  
  const auto resource_manager = gl_pipeline->GetResourceManager();
  const auto context_manager = gl_pipeline->GetContextManager();
  
  for (const auto &it : schema.targets) {
    const auto &target_schema = it.second;
    const auto &source_id = target_schema.source_id;
    const auto &target_id = target_schema.target_id;
    const auto &target_type = target_schema.type;
    const auto &model_id = target_schema.model_id;
    const auto &window_id = target_schema.window_id;
    const bool provided_model_id = target_schema.provided_model_id;
    
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
    
    result.result.targets.emplace(target_id, target);
  }
  
  result.status.success = true;
  
  return result;
}