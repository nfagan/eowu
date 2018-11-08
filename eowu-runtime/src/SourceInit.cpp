//
//  SourceInit.cpp
//  eowu-common
//
//  Created by Nick Fagan on 9/15/18.
//

#include "SourceInit.hpp"
#include <eowu-gl/eowu-gl.hpp>
#include <eowu-script/eowu-script.hpp>

#define EOWU_SOURCE_RESULT_EARLY_RETURN(from, to) \
  if (!from.status.success) { \
    to.status.message = from.status.message; \
    to.status.context = from.status.context; \
    return to; \
  }

eowu::init::SourceResult eowu::init::initialize_sources(const eowu::schema::Sources &schema,
                                                        std::shared_ptr<eowu::GLPipeline> gl_pipeline) {
  eowu::init::SourceResult result;
  
  //  get xy sources
  auto xy_source_result = eowu::init::initialize_xy_sources(schema, gl_pipeline);
  EOWU_SOURCE_RESULT_EARLY_RETURN(xy_source_result, result);
  
  result.result.xy_source_init = std::move(xy_source_result.result);
  
  result.status.success = true;
  
  return result;
}

eowu::init::XYSourceResult eowu::init::initialize_xy_sources(const eowu::schema::Sources &schema,
                                                             std::shared_ptr<eowu::GLPipeline> gl_pipeline) {
  eowu::init::XYSourceResult result;
  
  const auto context_manager = gl_pipeline->GetContextManager();
  
  for (const auto &it : schema.sources) {
    const auto &source = it.second;
    const auto &type = source.type;
    const auto &id = source.source_id;
    const auto &window_id = source.window_id;
    
    if (type == "Mouse") {
      //  this will throw if `window_id` is not a recognized alias, but validation
      //  should be previously handled.
      const auto &win = context_manager->GetWindowByAlias(window_id);
      const eowu::XYSource *source = &win->GetMouse();
      
      result.result.xy_sources.emplace(id, source);
      result.result.xy_source_window_mapping.emplace(id, window_id);
    }
  }
  
  result.status.success = true;
  
  return result;
}

