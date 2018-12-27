//
//  GLInit.hpp
//  eowu-common
//
//  Created by Nick Fagan on 9/5/18.
//

#pragma once

#include "SetupResult.hpp"
#include <eowu-script/eowu-script.hpp>
#include <cstddef>
#include <string>
#include <functional>
#include <memory>
#include <unordered_map>

namespace eowu {
  class Mesh;
  class TextModel;
  class GLPipeline;
  class ResourceManager;
  class FontFace;
  
  namespace init {
    struct GLInit {
      std::unordered_map<std::string, std::shared_ptr<eowu::TextModel>> text_models;
    };
    
    using MeshFactoryFunction = std::function<void(eowu::Mesh*)>;
    using GLInitResult = eowu::SetupResult<GLInit>;
    
    GLInitResult initialize_gl_pipeline(std::shared_ptr<eowu::GLPipeline> gl_pipeline,
                                        const eowu::schema::Setup &schema);
    
    void create_resources(std::shared_ptr<eowu::GLPipeline> gl_pipeline, const eowu::schema::Setup &schema);
    void create_models(std::shared_ptr<eowu::GLPipeline> gl_pipeline, const eowu::schema::Setup &schema);
    void create_meshes(std::shared_ptr<eowu::GLPipeline> gl_pipeline, const eowu::schema::Setup &schema);
    void create_textures(std::shared_ptr<eowu::GLPipeline> gl_pipeline, const eowu::schema::Setup &schema);
    void initialize_glfw(std::shared_ptr<eowu::GLPipeline> gl_pipeline);
    void open_windows(std::shared_ptr<eowu::GLPipeline> gl_pipeline, const eowu::schema::Setup &schema);
    //
    std::unordered_map<std::string, std::shared_ptr<eowu::TextModel>>
    initialize_fonts(std::shared_ptr<eowu::GLPipeline> gl_pipeline, const eowu::schema::Fonts &fonts);
    
    std::shared_ptr<eowu::TextModel> create_text_model();
    
    std::unordered_map<std::string, eowu::init::MeshFactoryFunction> get_geometry_to_mesh_factory_map();
    std::string get_message_n_of_n(std::size_t iter, std::size_t size);
  }
}


