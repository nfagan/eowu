//
//  TargetInit.hpp
//  eowu-common
//
//  Created by Nick Fagan on 9/15/18.
//

#pragma once

#include "SetupResult.hpp"
#include "SourceInit.hpp"
#include <unordered_map>
#include <string>
#include <memory>

namespace eowu {
  
  class GLPipeline;
  class XYTarget;
  
  namespace schema {
    struct Targets;
  }
  
  namespace init {
    using TargetMap = std::unordered_map<std::string, std::shared_ptr<eowu::XYTarget>>;
    
    struct TargetInit {
      TargetMap targets;
    };
    
    struct TargetResult : eowu::SetupResult<TargetInit> {};
    
    eowu::init::TargetResult initialize_targets(const eowu::schema::Targets &schema,
                                                const eowu::init::XYSourceMapType &xy_sources,
                                                std::shared_ptr<eowu::GLPipeline> gl_pipeline);
  }
}
