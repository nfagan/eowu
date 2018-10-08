//
//  GLInit.cpp
//  eowu-common
//
//  Created by Nick Fagan on 9/5/18.
//

#include "GLInit.hpp"
#include "Contexts.hpp"
#include "Util.hpp"
#include <eowu-gl/eowu-gl.hpp>
#include <stdexcept>
#include <unordered_map>

#define EOWU_SETUP_STATUS_EARLY_RETURN(name, res) \
  if (!name.success) { \
    res = name; \
    return res; \
  }

eowu::SetupStatus eowu::init::initialize_gl_pipeline(std::shared_ptr<eowu::GLPipeline> gl_pipeline, const eowu::schema::Setup &schema) {
  
  eowu::SetupStatus result;
  
  //  glfw init
  auto glfw_init = init::initialize_glfw(gl_pipeline);
  EOWU_SETUP_STATUS_EARLY_RETURN(glfw_init, result);
  
  //  window init
  auto window_init = init::open_windows(gl_pipeline, schema);
  EOWU_SETUP_STATUS_EARLY_RETURN(window_init, result);
  
  //  resource init
  auto resource_init = init::create_resources(gl_pipeline, schema);
  EOWU_SETUP_STATUS_EARLY_RETURN(resource_init, result);
  
  result.success = true;
  
  return result;
}

std::string eowu::init::get_message_n_of_n(std::size_t iter, std::size_t size) {
  return std::to_string(iter) + " of " + std::to_string(size);
}

eowu::SetupStatus eowu::init::create_textures(std::shared_ptr<eowu::GLPipeline> gl_pipeline,
                                              const eowu::schema::Setup &schema) {
  eowu::SetupStatus result;
  
  auto texture_manager = gl_pipeline->GetTextureManager();
  
  const auto &textures = schema.textures.mapping;
  
  const std::size_t n_textures = textures.size();
  std::size_t current_texture = 1;
  
  for (const auto &it : textures) {
    const std::string &id = it.first;
    const std::string &filename = it.second;
    
    EOWU_LOG_INFO("init::create_texture: Loading file " + get_message_n_of_n(current_texture, n_textures));
    
    try {
      texture_manager->LoadImage(filename, id);
    } catch (const std::exception &e) {
      result.message = e.what();
      result.context = std::string(eowu::contexts::gl_init) + "::CreateTextures";
      
      return result;
    }
    
    current_texture++;
  }
  
  result.success = true;
  
  return result;
}

eowu::SetupStatus eowu::init::create_meshes(std::shared_ptr<eowu::GLPipeline> gl_pipeline, const eowu::schema::Setup &schema) {
  eowu::SetupStatus result;
  
  auto resource_manager = gl_pipeline->GetResourceManager();
  auto mesh_factory_functions = eowu::init::get_geometry_to_mesh_factory_map();
  
  const auto &geometries = schema.geometry.builtins.mapping;
  
  for (const auto &it : geometries) {
    const auto &mesh_id = it.first;
    const auto &mesh_type = it.second;
    
    const auto &mesh_func_it = mesh_factory_functions.find(mesh_type);
    
    //  if we missed something in validation, this will catch it.
    if (mesh_func_it == mesh_factory_functions.end()) {
      result.message = "Internal error: no mesh implementation for mesh type '" + mesh_type + "'.";
      result.context = eowu::contexts::gl_init + std::string("::create_models");
      
      return result;
    }
    
    auto mesh = resource_manager->Create<eowu::Mesh>(mesh_id);
    
    //  call the mesh_factory function mapped to this mesh_type.
    mesh_func_it->second(mesh.get());
  }
  
  result.success = true;
  
  return result;
}

eowu::SetupStatus eowu::init::create_models(std::shared_ptr<eowu::GLPipeline> gl_pipeline, const eowu::schema::Setup &schema) {
  eowu::SetupStatus result;
  
  std::unordered_map<std::string, std::shared_ptr<eowu::Mesh>> meshes;
  
  auto resource_manager = gl_pipeline->GetResourceManager();
  auto texture_manager = gl_pipeline->GetTextureManager();
  
  const auto &stimuli = schema.stimuli.stimuli;
  
  //  models
  for (const auto &it : stimuli) {
    
    const auto &stim_schema = it.second;
    
    const std::string &stim_id = stim_schema.stimulus_id;
    
    //  handle mesh assignment
    std::shared_ptr<eowu::Mesh> mesh = nullptr;
    
    if (stim_schema.provided_geometry_id) {
      const std::string &mesh_id = stim_schema.geometry_id;
      
      try {
        mesh = resource_manager->Get<eowu::Mesh>(mesh_id);
      } catch (const std::exception &e) {
        result.message = e.what();
        result.context = "init::create_models";
        
        return result;
      }
    }
    
    //  handle material + model creation
    auto material = resource_manager->Create<eowu::Material>(stim_id);
    auto stim = resource_manager->Create<eowu::Model>(stim_id, mesh, material);
    
    if (stim_schema.provided_texture_id) {
      auto tex = texture_manager->Get(stim_schema.texture_id);
      material->SetFaceColor(tex);
    } else {
      material->SetFaceColor(eowu::util::require_vec3(stim_schema.color));
    }
    
    //  handle stimulus placement
    auto &trans = stim->GetTransform();
    
    unsigned int units;
    
    try {
      units = eowu::units::get_units_from_string_label(stim_schema.units);
    } catch (const std::exception &e) {
      result.message = e.what();
      result.context = eowu::contexts::gl_init + std::string("::GetUnits");
    }
    
    auto pos = eowu::util::require_vec3(stim_schema.position);
    auto rot = eowu::util::require_vec3(stim_schema.rotation);
    auto scl = eowu::util::require_vec3(stim_schema.size);
    
    trans.SetPosition(pos);
    trans.SetScale(scl);
    trans.SetRotation(rot);
    trans.SetUnits(units);
  }
  
  result.success = true;
  
  return result;
}

eowu::SetupStatus eowu::init::create_resources(std::shared_ptr<eowu::GLPipeline> gl_pipeline, const eowu::schema::Setup &schema) {
  
  eowu::SetupStatus result;
  
  //  texture init
  auto tex_init = init::create_textures(gl_pipeline, schema);
  EOWU_SETUP_STATUS_EARLY_RETURN(tex_init, result);
  
  //  mesh init
  auto mesh_init = init::create_meshes(gl_pipeline, schema);
  EOWU_SETUP_STATUS_EARLY_RETURN(mesh_init, result);
  
  //  model init
  auto model_init = init::create_models(gl_pipeline, schema);
  EOWU_SETUP_STATUS_EARLY_RETURN(model_init, result);
  
  result.success = true;
  
  return result;
}

eowu::SetupStatus eowu::init::initialize_glfw(std::shared_ptr<eowu::GLPipeline> gl_pipeline) {
  eowu::SetupStatus result;
  
  auto context_manager = gl_pipeline->GetContextManager();
  
  try {
    context_manager->Initialize();
  } catch (const std::exception &e) {
    result.message = e.what();
    result.context = eowu::contexts::gl_init;
    
    return result;
  }
  
  result.success = true;
  
  return result;
}

eowu::SetupStatus eowu::init::open_windows(std::shared_ptr<eowu::GLPipeline> gl_pipeline, const eowu::schema::Setup &schema) {
  eowu::SetupStatus result;
  
  auto context_manager = gl_pipeline->GetContextManager();
  auto window_container = gl_pipeline->GetWindowContainer();
  
  const auto &window_mapping = schema.windows.windows;
  
  for (const auto &win : window_mapping) {
    const std::string &win_id = win.first;
    const eowu::schema::Window &input_spec = win.second;
    eowu::WindowProperties open_spec;
    
    open_spec.is_fullscreen = input_spec.is_fullscreen;
    open_spec.is_resizeable = input_spec.is_resizeable;
    open_spec.swap_interval = input_spec.is_vsynced ? 1 : 0;
    open_spec.index = input_spec.index;
    open_spec.width = input_spec.width;
    open_spec.height = input_spec.height;
    open_spec.title = input_spec.title;
    
    try {
      auto win = context_manager->OpenWindow(open_spec);
      win->SetAlias(win_id);
      
      window_container->Emplace(win_id, win);
    } catch (const std::exception &e) {
      result.message = e.what();
      result.context = eowu::contexts::gl_init + std::string("::OpenWindow");
      
      return result;
    }
  }
  
  result.success = true;
  
  return result;
}

std::unordered_map<std::string, eowu::init::MeshFactoryFunction> eowu::init::get_geometry_to_mesh_factory_map() {  
  return {
    {"Rectangle", &eowu::mesh_factory::make_quad},
    {"Circle", &eowu::mesh_factory::make_default_sphere},
    {"Triangle", &eowu::mesh_factory::make_triangle},
    {"RectangleFrame", &eowu::mesh_factory::make_frame_quad},
    {"CircleFrame", &eowu::mesh_factory::make_frame_circle},
    {"TriangleFrame", &eowu::mesh_factory::make_frame_triangle}
  };
}
