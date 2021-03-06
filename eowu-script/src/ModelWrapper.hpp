//
//  ModelWrapper.hpp
//  eowu-script
//
//  Created by Nick Fagan on 9/1/18.
//

#pragma once

#include "Lua.hpp"
#include <memory>
#include <string>

namespace eowu {
  class Model;
  class Renderer;
  class TextureManager;
  class WindowContainerMap;
  class ResourceManager;
  
  class ModelWrapper;
  class VectorWrapper;
}

class eowu::ModelWrapper {
  
public:
  ModelWrapper(std::shared_ptr<eowu::Model> model,
               std::shared_ptr<eowu::ResourceManager> resource_manager,
               std::shared_ptr<eowu::Renderer> renderer,
               std::shared_ptr<eowu::WindowContainerMap> window_container,
               std::shared_ptr<eowu::TextureManager> texture_manager);
  
  ~ModelWrapper() = default;
  
  int SetColor(lua_State *L);
  void SetTexture(const std::string &id);
  void SetOpacity(double value);
  void SetGeometry(const std::string &id);
  void MakeLike(const eowu::ModelWrapper *other);
  
  int SetPositionVector(lua_State *L);
  int SetRotationVector(lua_State *L);
  int SetScaleVector(lua_State *L);
  
  void SetPosition(const eowu::VectorWrapper &vec);
  void SetRotation(const eowu::VectorWrapper &vec);
  void SetScale(const eowu::VectorWrapper &vec);
  void SetUnits(const std::string &units);
  void SetZRotation(double value);
  
  int Move(lua_State *L);
  int Rotate(lua_State *L);
  int Scale(lua_State *L);
  void ZRotate(double amount);
  
  int Draw(lua_State *L);
  
  eowu::VectorWrapper GetPosition() const;
  eowu::VectorWrapper GetRotation() const;
  eowu::VectorWrapper GetScale() const;
  
  static void CreateLuaSchema(lua_State *L);
private:
  std::shared_ptr<eowu::Model> model;
  std::shared_ptr<eowu::ResourceManager> resource_manager;
  std::shared_ptr<eowu::Renderer> renderer;
  std::shared_ptr<eowu::TextureManager> texture_manager;
  std::shared_ptr<eowu::WindowContainerMap> window_container;
  
  void assert_mesh();
  void assert_material();
};
