//
//  SourceInit.hpp
//  eowu-common
//
//  Created by Nick Fagan on 9/15/18.
//

#pragma once

#include "SetupResult.hpp"
#include <unordered_map>
#include <string>
#include <memory>

namespace eowu {
  class XYSource;
  class GLPipeline;
  
  namespace schema {
    struct Sources;
  }
  
  namespace init {
    using XYSourceMapType = std::unordered_map<std::string, const eowu::XYSource*>;
    using XYSetupResult = eowu::SetupResult<XYSourceMapType>;
    
    struct SourceInit {
      XYSourceMapType xy_sources;
    };
    struct SourceResult : eowu::SetupResult<SourceInit> {};
    
    eowu::init::SourceResult initialize_sources(const eowu::schema::Sources &schema,
                                                  std::shared_ptr<eowu::GLPipeline> gl_pipeline);
    XYSetupResult initialize_xy_sources(const eowu::schema::Sources &schema,
                                        std::shared_ptr<eowu::GLPipeline> gl_pipeline);
  }
}
