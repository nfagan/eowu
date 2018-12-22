//
//  XYSourceWrapper.cpp
//  eowu-common
//
//  Created by Nick Fagan on 12/22/18.
//

#include "XYSourceWrapper.hpp"
#include "VectorWrapper.hpp"
#include "Constants.hpp"
#include "Lua.hpp"
#include <eowu-gl/eowu-gl.hpp>

eowu::XYSourceWrapper::XYSourceWrapper(const eowu::XYSource *src) :
source(src) {
  //
}

double eowu::XYSourceWrapper::GetX() const {
  return source->GetLatestSample().x;
}

double eowu::XYSourceWrapper::GetY() const {
  return source->GetLatestSample().y;
}

eowu::VectorWrapper eowu::XYSourceWrapper::GetPosition() const {
  auto sample = source->GetLatestSample();
  glm::vec3 position(sample.x, sample.y, 0.0);
  eowu::VectorWrapper vector(position);
  
  return vector;
}

void eowu::XYSourceWrapper::CreateLuaSchema(lua_State *L) {
  luabridge::getGlobalNamespace(L)
  .beginNamespace(eowu::constants::eowu_namespace)
  .beginClass<eowu::XYSourceWrapper>("_XYSource")
  .addProperty("x", &eowu::XYSourceWrapper::GetX)
  .addProperty("y", &eowu::XYSourceWrapper::GetY)
  .addProperty("position", &eowu::XYSourceWrapper::GetPosition)
  .endClass()
  .endNamespace();
}
