//
//  ModelWrapper.hpp
//  eowu-script
//
//  Created by Nick Fagan on 9/1/18.
//

#pragma once

#include <memory>

struct lua_State;

namespace eowu {
  class Model;
  class Renderer;
  
  class ModelWrapper;
  class VectorWrapper;
}

class eowu::ModelWrapper {
  
public:
  ModelWrapper(std::shared_ptr<eowu::Model> model, std::shared_ptr<eowu::Renderer> renderer);
  ~ModelWrapper() = default;
  
  void SetColor(double r, double b, double g);
  
  void SetPosition(const eowu::VectorWrapper &vec);
  void SetRotation(const eowu::VectorWrapper &vec);
  void SetScale(const eowu::VectorWrapper &vec);
  void SetUnits(const std::string &units);
  
  void Draw(lua_State *L);
  
  eowu::VectorWrapper GetPosition() const;
  eowu::VectorWrapper GetRotation() const;
  eowu::VectorWrapper GetScale() const;
  
  static void CreateLuaSchema(lua_State *L);
private:
  std::shared_ptr<eowu::Model> model;
  std::shared_ptr<eowu::Renderer> renderer;
  
  void assert_mesh();
  void assert_material();
};
