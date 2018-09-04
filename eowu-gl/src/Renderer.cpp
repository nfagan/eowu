//
//  Renderer.cpp
//  eowu-gl
//
//  Created by Nick Fagan on 8/29/18.
//

#include <glad/glad.h>
#include "Renderer.hpp"
#include "Error.hpp"
#include "Material.hpp"
#include "ShaderBuilder.hpp"
#include "UniformNames.hpp"
#include <eowu-common/logging.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <utility>
#include <cstddef>

#ifdef EOWU_DEBUG
#include <iostream>
#endif

eowu::Renderer::Renderer(ContextContainerType context_manager) {
  this->context_manager = context_manager;
  this->projection_type = eowu::projection_types::orthographic;
  this->last_program = nullptr;
  this->last_mesh = nullptr;
}

void eowu::Renderer::ClearQueue() {
  std::lock_guard<std::mutex> guard(models_mutex);
  
  models.clear();
}

void eowu::Renderer::Queue(const ModelAggregateType &models) {
  std::lock_guard<std::mutex> guard(models_mutex);
  
  for (const auto& model : models) {
    this->models.push_back(model);
  }
}

void eowu::Renderer::Queue(const eowu::Model &model) {
  std::lock_guard<std::mutex> guard(models_mutex);
  models.push_back(model);
}

void eowu::Renderer::draw(eowu::WindowType window) {
  std::lock_guard<std::mutex> guard(models_mutex);
  
  window->MakeCurrent();
  
  if (window->WasResized()) {
    auto fb_size = window->GetFramebufferSize();
    glViewport(0, 0, fb_size.x, fb_size.y);
    window->ResetWasResized();
  }
  
  glClear(GL_COLOR_BUFFER_BIT);
  
  for (const auto& model : models) {
    draw_one_model(window, model);
  }
  
  window->SwapBuffers();
  
  last_window = window;
}

void eowu::Renderer::next_frame() {
  for (auto &model : models) {
    model.NextFrame();
  }
}

void eowu::Renderer::Draw(eowu::WindowType window) {
  draw(window);
  next_frame();
}

void eowu::Renderer::Draw(const eowu::WindowContainerType& windows) {
  for (const auto win : windows) {
    draw(win);
  }
  next_frame();
}

void eowu::Renderer::draw_one_model(const eowu::WindowType& window, const eowu::Model &model) {
  
  auto mesh = model.GetMesh();
  auto material = model.GetMaterial();
  auto transform = model.GetTransform();
  
  if (!material || !mesh) {
    EOWU_LOG_INFO("Renderer::draw_one_model: Material or Mesh was null.");
    return;
  }
  
  transform.SetScreenDimensions(window->GetDimensions());
  
  auto window_id = window->GetIdentifier();
  auto material_id = material->GetIdentifier();
  auto mesh_id = mesh->GetIdentifier();
  bool has_program_for_material = programs_by_material_id.count(material_id) > 0;
  bool material_schema_changed = material->SchemaChanged();
  bool prog_need_bind = false;
  
  std::shared_ptr<eowu::Program> prog = nullptr;
  
  if (!has_program_for_material || material_schema_changed) {
    //  analyze material to see whether we need to make
    //  a new shader
    EOWU_LOG_INFO("Renderer::draw_one_model: Analyzing material.");
    
    const eowu::Mesh &ref_mesh = *(mesh.get());
    const eowu::Material &ref_mat = *(material.get());
    
    std::string v_src = eowu::builder::vertex(ref_mesh, ref_mat);
    std::string f_src = eowu::builder::fragment(ref_mesh, ref_mat);
    
    std::hash<std::string> hash_fnc;
    std::size_t hash_code = hash_fnc(v_src + f_src);
    
    bool prog_exists = programs.count(hash_code) > 0;
    
    if (prog_exists) {
      prog = programs.at(hash_code);
      EOWU_LOG_INFO("Renderer::draw_one_model: Using cached shader.");
    } else {
      EOWU_LOG_INFO("Renderer::draw_one_model: Generating new program.");
      prog = eowu::builder::from_source(v_src, f_src);
    }
    
    programs[hash_code] = prog;
    programs_by_material_id[material_id] = hash_code;
    
    prog_need_bind = true;
  } else {
    prog = programs.at(programs_by_material_id.at(material_id));
    
    bool is_new_prog = last_program->GetIdentifier() != prog->GetIdentifier();
    bool is_new_window = !last_window || last_window->GetIdentifier() != window_id;
    
    prog_need_bind = is_new_prog || is_new_window;
  }
  
  if (prog_need_bind) {
    prog->Bind();
  }
  
  material->Configure(*(prog.get()));
  
  auto trans = transform.GetTransformationMatrix();
  auto proj = get_projection_matrix(window);
  
  prog->SetUniform(eowu::uniforms::model, trans);
  prog->SetUniform(eowu::uniforms::view, glm::mat4(1.0f));
  prog->SetUniform(eowu::uniforms::projection, proj);
  
  bool mesh_need_bind = prog_need_bind || !last_mesh || last_mesh->GetIdentifier() != mesh_id;
  
  if (mesh_need_bind) {    
    if (last_mesh) {
      last_mesh->Unbind(window_id);
    }
    
    mesh->Bind(window_id);
  }
  
  mesh->Draw();
  
  last_program = prog;
  last_mesh = mesh;
}

glm::mat4 eowu::Renderer::get_projection_matrix(eowu::WindowType window) const {
  
  if (projection_type == eowu::projection_types::orthographic) {
    return glm::ortho(0.0f, (float)window->GetWidth(), 0.0f, (float)window->GetHeight());
  } else {
    throw eowu::NotImplementedError("Non-orthographic projections not yet implemented.");
  }
  
}
