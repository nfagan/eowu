//
//  Renderer.cpp
//  eowu-gl
//
//  Created by Nick Fagan on 8/29/18.
//

#include "Renderer.hpp"
#include "Error.hpp"
#include "Material.hpp"
#include "ShaderBuilder.hpp"
#include "UniformNames.hpp"
#include <eowu-common/logging.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <utility>
#include <cstddef>
#include <glad/glad.h>

#ifdef EOWU_DEBUG
#include <iostream>
#endif

namespace priv {
  static void buffer_swap_noop(const eowu::WindowType &win) {
    //
  }
}

eowu::Renderer::Renderer() :
on_buffer_swap(&priv::buffer_swap_noop),
projection_type(eowu::projection_types::orthographic),
clear_color(glm::vec3(0.0)),
last_program(nullptr),
last_mesh(nullptr) {
  //
}

void eowu::Renderer::ClearQueue() {
  std::lock_guard<std::mutex> guard(mut);
  
  for (auto &it : models) {
    it.second.clear();
  }
}

void eowu::Renderer::Queue(const ModelAggregateType &models, eowu::WindowType window) {
  std::lock_guard<std::mutex> guard(mut);
  
  auto &models_container = get_models_container(window);
  
  for (const auto& model : models) {
    models_container.push_back(model);
  }
}

void eowu::Renderer::Queue(const eowu::Model &model, eowu::WindowType window) {
  std::lock_guard<std::mutex> guard(mut);
  
  std::vector<eowu::Model> &models_container = get_models_container(window);
  models_container.push_back(model);
}

void eowu::Renderer::SetClearColor(const glm::vec3 &color) {
  std::lock_guard<std::mutex> lock(mut);
  
  clear_color = color;
}

eowu::time::DurationType eowu::Renderer::Delta() {
  std::lock_guard<std::mutex> guard(mut);
  
  auto ellapsed = frame_timing.timer.Elapsed();
  frame_timing.timer.Reset();
  
  return ellapsed;
}

void eowu::Renderer::draw(eowu::WindowType window) {
  std::lock_guard<std::mutex> guard(mut);
  
  if (!window->IsOpen()) {
    return;
  }
  
  window->MakeCurrent();
  window->ApplySwapInterval();
  
  if (window->WasResized()) {
    auto fb_size = window->GetFramebufferSize();
    glViewport(0, 0, fb_size.x, fb_size.y);
    window->ResetWasResized();
  }
  
  glClearColor(clear_color.x, clear_color.y, clear_color.z, 1.0);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glEnable(GL_BLEND);
  
  auto &models_container = get_models_container(window);
  
  for (const auto& model : models_container) {
    draw_one_model(window, model);
  }
  
  window->SwapBuffers();
  //  happens immediately after buffer was swapped
  on_buffer_swap(window);
  
  last_window = window;
}

void eowu::Renderer::next_frame() {
  for (const auto &it : windows) {
    auto &models_container = get_models_container(it.second);
    
    for (auto &model : models_container) {
      model.NextFrame();
    }
  }
  
  analyzed_material_ids.clear();
}

void eowu::Renderer::Draw() {  
  for (const auto &it : windows) {
    draw(it.second);
  }
  
  next_frame();
}

void eowu::Renderer::draw_one_model(const eowu::WindowType& window, const eowu::Model &model) {
  auto mesh = model.GetMesh();
  auto material = model.GetMaterial();
  auto transform = model.GetTransform();
  
  if (!material || !mesh) {
    EOWU_LOG_WARN("Renderer::draw_one_model: Material or Mesh was null.");
    return;
  }
  
  transform.SetScreenDimensions(window->GetDimensions());
  
  auto window_id = window->GetIdentifier();
  auto material_id = material->GetIdentifier();
  auto mesh_id = mesh->GetIdentifier();
  bool has_program_for_material = programs_by_material_id.count(material_id) > 0;
  bool material_schema_changed = material->SchemaChanged();
  bool material_analyzed = analyzed_material_ids.count(material_id) > 0;
  bool prog_need_bind = false;
  bool material_needs_check = !material_analyzed && (!has_program_for_material || material_schema_changed);
  
  std::shared_ptr<eowu::Program> prog = nullptr;
  
  if (material_needs_check) {
    //  analyze material to see whether we need to make
    //  a new shader
    const eowu::Mesh &ref_mesh = *(mesh.get());
    const eowu::Material &ref_mat = *(material.get());
    
    std::string v_src = eowu::builder::vertex(ref_mesh, ref_mat);
    std::string f_src = eowu::builder::fragment(ref_mesh, ref_mat);
    
    std::hash<std::string> hash_fnc;
    std::size_t hash_code = hash_fnc(v_src + f_src);
    
    bool prog_exists = programs.count(hash_code) > 0;
    
    if (prog_exists) {
      prog = programs.at(hash_code);
      
      bool is_new_prog = !last_program || last_program->GetIdentifier() != prog->GetIdentifier();
      bool is_new_window = !last_window || last_window->GetIdentifier() != window_id;
      
      prog_need_bind = is_new_prog || is_new_window;
    } else {
      prog = eowu::builder::from_source(v_src, f_src);
      prog_need_bind = true;
    }
    
    programs[hash_code] = prog;
    programs_by_material_id[material_id] = hash_code;
    analyzed_material_ids.emplace(material_id);
    
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
  auto view = get_view_matrix();
  
  prog->SetUniform(eowu::uniforms::model, trans);
  prog->SetUniform(eowu::uniforms::view, view);
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

std::vector<eowu::Model>& eowu::Renderer::get_models_container(const eowu::WindowType &win) {
  
  const auto& win_id = win->GetIdentifier();
  const auto& window_it = windows.find(win_id);
  
  if (window_it == windows.end()) {
    windows.emplace(win_id, win);
    models.emplace(win_id, std::vector<eowu::Model>());
  }
  
  return models.at(win_id);
}

glm::mat4 eowu::Renderer::get_view_matrix() const {
  return glm::mat4(1.0);
}

glm::mat4 eowu::Renderer::get_projection_matrix(eowu::WindowType window) const {
  if (projection_type == eowu::projection_types::orthographic) {
    return glm::ortho(0.0f, (float)window->GetWidth(), 0.0f, (float)window->GetHeight());
  } else {
    throw eowu::NotImplementedError("Non-orthographic projections not yet implemented.");
  }
}

void eowu::Renderer::SetOnBufferSwap(const eowu::BufferSwapCallbackType &cb) {
  std::lock_guard<std::mutex> guard(mut);
  
  on_buffer_swap = cb;
}
