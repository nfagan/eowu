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
  RendererWrapper(std::shared_ptr<eowu::Renderer> renderer,
                  std::shared_ptr<eowu::LuaFunction> lua_render_function);
  
  int SetRenderFunction(lua_State *L);
  
  static void CreateLuaSchema(lua_State *L);
private:
  std::shared_ptr<eowu::Renderer> renderer;
  std::shared_ptr<eowu::LuaFunction> lua_render_function;
};
