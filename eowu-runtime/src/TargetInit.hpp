//
//  TargetInit.hpp
//  eowu-common
//
//  Created by Nick Fagan on 9/15/18.
//

#pragma once

#include "SetupResult.hpp"
#include "SourceInit.hpp"
#include "GLInit.hpp"
#include <unordered_map>
#include <string>
#include <memory>

namespace eowu {
  
  class GLPipeline;
  class XYTarget;
  class Model;
  class TargetWrapper;
  class LuaContext;
  class ResourceManager;
  
  namespace schema {
    struct Targets;
  }
  
  namespace init {
    using TargetMap = std::unordered_map<std::string, std::shared_ptr<eowu::XYTarget>>;
    using TargetWrapperMapType = std::unordered_map<std::string, std::shared_ptr<eowu::TargetWrapper>>;
    
    struct TargetInit {
      TargetMap targets;
      TargetWrapperMapType target_wrappers;
    };
    
    struct TargetResult : eowu::SetupResult<TargetInit> {};
    
    eowu::init::TargetResult initialize_targets(const eowu::schema::Targets &schema,
                                                const eowu::init::XYSourceMapType &xy_sources,
                                                std::shared_ptr<eowu::LuaContext> lua_context,
                                                std::shared_ptr<eowu::GLPipeline> gl_pipeline);
    
    std::unordered_map<std::string, eowu::init::MeshFactoryFunction> get_target_type_to_mesh_factory_map();
    std::string get_target_id(const std::shared_ptr<eowu::ResourceManager> &resource_manager);
  }
}
