//
//  MathWrapper.cpp
//  eowu-common
//
//  Created by Nick Fagan on 9/9/18.
//

#include "MathWrapper.hpp"
#include "Constants.hpp"
#include "Lua.hpp"
#include <cmath>

double eowu::MathWrapper::nan() {
  return std::nan("");
}

bool eowu::MathWrapper::isnan(double value) {
  return value != value;
}

void eowu::MathWrapper::CreateLuaSchema(lua_State *L) {
  luabridge::getGlobalNamespace(L)
  .beginNamespace(eowu::constants::eowu_namespace)
  .beginClass<eowu::MathWrapper>(eowu::constants::eowu_math_namespace)
  .addStaticFunction("nan", &eowu::MathWrapper::nan)
  .addStaticFunction("isnan", &eowu::MathWrapper::isnan)
  .endClass()
  .endNamespace();
}
