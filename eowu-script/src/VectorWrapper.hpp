//
//  VectorWrapper.hpp
//  eowu-script
//
//  Created by Nick Fagan on 9/1/18.
//

#pragma once

#include <glm/glm.hpp>

struct lua_State;

namespace eowu {
  class VectorWrapper;
}

class eowu::VectorWrapper {
public:
  VectorWrapper();
  VectorWrapper(const glm::vec3 &vec);
  
  ~VectorWrapper() = default;
  
  void SetX(double x);
  void SetY(double y);
  void SetZ(double z);
  
  double GetX() const;
  double GetY() const;
  double GetZ() const;
  
  const glm::vec3& GetValue() const;
  
  static void CreateLuaSchema(lua_State *L);
private:
  glm::vec3 value;
};

