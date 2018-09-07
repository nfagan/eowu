//
//  RendererWrapper.hpp
//  eowu-common
//
//  Created by Nick Fagan on 9/3/18.
//

#pragma once

#include <memory>

struct lua_State;

namespace eowu {
  class RendererWrapper;
  class Renderer;
  class LuaFunction;
}

class eowu::RendererWrapper {
  
public:
  RendererWrapper(std::shared_ptr<eowu::Renderer> renderer);
  
  int SetClearColor(lua_State *L);
  double Delta();
  
  static void CreateLuaSchema(lua_State *L);
private:
  std::shared_ptr<eowu::Renderer> renderer;
};
