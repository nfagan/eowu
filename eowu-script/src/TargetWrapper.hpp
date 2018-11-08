//
//  TargetWrapper.hpp
//  eowu-common
//
//  Created by Nick Fagan on 9/17/18.
//

#pragma once

#include "LuaFunction.hpp"
#include <memory>

namespace eowu {
  class XYTarget;
  class Model;
  class LuaContext;
  class Renderer;
  class WindowContainerMap;
  
  class TargetWrapper;
}

class eowu::TargetWrapper {
public:
  TargetWrapper(std::shared_ptr<eowu::LuaContext> lua_context,
                std::shared_ptr<eowu::XYTarget> target,
                std::shared_ptr<eowu::Model> model,
                std::shared_ptr<eowu::Renderer> renderer,
                std::shared_ptr<eowu::WindowContainerMap> window_container);
  ~TargetWrapper() = default;
  
  void Reset();
  void Show();
  void Hide();
  
  bool IsInBounds() const;
  
  int Draw(lua_State *L);
  
  void SetOnEntry(luabridge::LuaRef func);
  void SetOnExit(luabridge::LuaRef func);
  
  static void CreateLuaSchema(lua_State *L);
private:
  std::shared_ptr<eowu::LuaContext> lua_context;
  std::shared_ptr<eowu::XYTarget> target;
  std::shared_ptr<eowu::Model> model;
  std::shared_ptr<eowu::Renderer> renderer;
  std::shared_ptr<eowu::WindowContainerMap> window_container;
  
  eowu::LuaFunction on_entry;
  eowu::LuaFunction on_exit;
  
  bool is_hidden;
  
  void configure_target_callbacks();
  void configure_entry_callback();
  void configure_exit_callback();
  
  void configure_model();
  
  void assert_not_part_of_set(const std::string &kind);
  static std::string get_part_of_set_error_message(const std::string &id, const std::string &kind);
};
