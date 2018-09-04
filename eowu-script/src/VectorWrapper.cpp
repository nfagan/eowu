//
//  VectorWrapper.cpp
//  eowu-script
//
//  Created by Nick Fagan on 9/1/18.
//

#include "VectorWrapper.hpp"
#include "Lua.hpp"
#include "Constants.hpp"

eowu::VectorWrapper::VectorWrapper() {
  value = glm::vec3(0.0, 0.0, 1.0);
}

eowu::VectorWrapper::VectorWrapper(const glm::vec3 &vec) {
  value = vec;
}

const glm::vec3& eowu::VectorWrapper::GetValue() const {
  return value;
}

double eowu::VectorWrapper::GetX() const {
  return value.x;
}

double eowu::VectorWrapper::GetY() const {
  return value.y;
}

double eowu::VectorWrapper::GetZ() const {
  return value.z;
}

void eowu::VectorWrapper::SetX(double x) {
  value.x = x;
}

void eowu::VectorWrapper::SetY(double y) {
  value.y = y;
}

void eowu::VectorWrapper::SetZ(double z) {
  value.z = z;
}

void eowu::VectorWrapper::CreateLuaSchema(lua_State *L) {
  luabridge::getGlobalNamespace(L)
  .beginNamespace(eowu::constants::eowu_namespace)
  .beginClass<eowu::VectorWrapper>("Vector")
  .addConstructor<void(*) (void)>()
  .addProperty("x", &eowu::VectorWrapper::GetX, &eowu::VectorWrapper::SetX)
  .addProperty("y", &eowu::VectorWrapper::GetY, &eowu::VectorWrapper::SetY)
  .addProperty("z", &eowu::VectorWrapper::GetZ, &eowu::VectorWrapper::SetZ)
  .endClass()
  .endNamespace();
}
