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

namespace eowu {
  namespace init {
    eowu::SetupStatus create_resources(std::shared_ptr<eowu::GLPipeline> gl_pipeline, const eowu::schema::Setup &schema);
    eowu::SetupStatus create_models(std::shared_ptr<eowu::GLPipeline> gl_pipeline, const eowu::schema::Setup &schema);
    eowu::SetupStatus create_textures(std::shared_ptr<eowu::GLPipeline> gl_pipeline, const eowu::schema::Setup &schema);
    eowu::SetupStatus initialize_gl_pipeline(std::shared_ptr<eowu::GLPipeline> gl_pipeline, const eowu::schema::Setup &schema);
    eowu::SetupStatus initialize_glfw(std::shared_ptr<eowu::GLPipeline> gl_pipeline);
    eowu::SetupStatus open_windows(std::shared_ptr<eowu::GLPipeline> gl_pipeline, const eowu::schema::Setup &schema);
    
    template<typename T>
    glm::vec3 require_vec3(const std::vector<T> &other);
  }
}

template<typename T>
glm::vec3 eowu::init::require_vec3(const std::vector<T> &other) {
  std::size_t n_els = other.size();
  glm::vec3 result;
  
  for (std::size_t i = 0; i < 3; i++) {
    result[i] = n_els <= i ? 0 : other[i];
  }
  
  return result;  
}
