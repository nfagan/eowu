//
//  GLInit.hpp
//  eowu-common
//
//  Created by Nick Fagan on 9/5/18.
//

#pragma once

#include "SetupStatus.hpp"
#include <eowu-script/eowu-script.hpp>
#include <glm/glm.hpp>
#include <cstddef>
#include <functional>
#include <string>
#include <unordered_map>

namespace eowu {
  class Mesh;
  
  namespace init {
    using MeshFactoryFunction = std::function<void(eowu::Mesh*)>;
    
    eowu::SetupStatus create_resources(std::shared_ptr<eowu::GLPipeline> gl_pipeline, const eowu::schema::Setup &schema);
    eowu::SetupStatus create_models(std::shared_ptr<eowu::GLPipeline> gl_pipeline, const eowu::schema::Setup &schema);
    eowu::SetupStatus create_meshes(std::shared_ptr<eowu::GLPipeline> gl_pipeline, const eowu::schema::Setup &schema);
    eowu::SetupStatus create_textures(std::shared_ptr<eowu::GLPipeline> gl_pipeline, const eowu::schema::Setup &schema);
    eowu::SetupStatus initialize_gl_pipeline(std::shared_ptr<eowu::GLPipeline> gl_pipeline, const eowu::schema::Setup &schema);
    eowu::SetupStatus initialize_glfw(std::shared_ptr<eowu::GLPipeline> gl_pipeline);
    eowu::SetupStatus open_windows(std::shared_ptr<eowu::GLPipeline> gl_pipeline, const eowu::schema::Setup &schema);
    
    std::unordered_map<std::string, eowu::init::MeshFactoryFunction> get_geometry_to_mesh_factory_map();
    std::string get_message_n_of_n(std::size_t iter, std::size_t size);
  }
}


