//
//  Renderer.hpp
//  eowu-gl
//
//  Created by Nick Fagan on 8/29/18.
//

#pragma once

#include "ContextManager.hpp"
#include "Model.hpp"
#include "ProjectionTypes.hpp"
#include "Identifier.hpp"
#include "Program.hpp"
#include <eowu-common/Timer.hpp>
#include <memory>
#include <mutex>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <cstddef>

namespace eowu {
  class Renderer;
  
  using ContextContainerType = std::shared_ptr<eowu::ContextManager>;
  using ModelAggregateType = std::vector<eowu::Model>;
}

class eowu::Renderer {
  
public:
  Renderer();
  
  void ClearQueue();
  void Queue(const ModelAggregateType &models, eowu::WindowType window);
  void Queue(const eowu::Model &model, eowu::WindowType window);
  void Draw();
  void SetClearColor(const glm::vec3 &color);
  eowu::time::DurationType Delta();
  
private:
  mutable std::mutex mut;

  eowu::projection_types::Types projection_type;
  glm::vec3 clear_color;
  
  struct FrameTime {
    eowu::Timer timer;
  } frame_timing;
  
  std::unordered_map<eowu::Identifier, eowu::WindowType> windows;
  std::unordered_map<eowu::Identifier, std::vector<Model>> models;
  
  std::unordered_map<std::size_t, std::shared_ptr<eowu::Program>> programs;
  std::unordered_map<eowu::Identifier, std::size_t> programs_by_material_id;
  std::unordered_set<eowu::Identifier> analyzed_material_ids;
  
  std::shared_ptr<eowu::Program> last_program;
  std::shared_ptr<eowu::Mesh> last_mesh;
  eowu::WindowType last_window;
  
  glm::mat4 get_projection_matrix(eowu::WindowType window) const;
  glm::mat4 get_view_matrix() const;
  
  std::vector<eowu::Model>& get_models_container(const eowu::WindowType &window);
  
  void next_frame();
  void draw(eowu::WindowType window);
  void draw_one_model(const eowu::WindowType &window, const eowu::Model &model);
};
