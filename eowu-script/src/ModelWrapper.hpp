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
  class CommandQueue;
  
  class ModelWrapper;
  class VectorWrapper;
}

class eowu::ModelWrapper {
  
public:
  ModelWrapper(std::shared_ptr<eowu::CommandQueue> command_queue, std::shared_ptr<eowu::Model> model);
  ~ModelWrapper() = default;
  
  void SetColor(double r, double b, double g);
  
  void SetPosition(const eowu::VectorWrapper &vec);
  void SetRotation(const eowu::VectorWrapper &vec);
  void SetScale(const eowu::VectorWrapper &vec);
  
  eowu::VectorWrapper GetPosition() const;
  eowu::VectorWrapper GetRotation() const;
  eowu::VectorWrapper GetScale() const;
  
  static void CreateLuaSchema(lua_State *L);
private:
  std::shared_ptr<eowu::Model> model;
  std::shared_ptr<eowu::CommandQueue> command_queue;
  
  void assert_mesh();
  void assert_material();
};
