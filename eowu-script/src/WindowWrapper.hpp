//
//  WindowWrapper.hpp
//  eowu-common
//
//  Created by Nick Fagan on 10/7/18.
//

#pragma once

#include <memory>
#include <string>

struct lua_State;

namespace eowu {
  class Window;
  
  class WindowWrapper;
}

class eowu::WindowWrapper {
public:
  WindowWrapper(std::shared_ptr<eowu::Window> window);
  ~WindowWrapper() = default;
  
  double GetWidth() const;
  double GetHeight() const;
  const std::string& GetAlias() const;
  
  static void CreateLuaSchema(lua_State *L);
  
private:
  std::shared_ptr<eowu::Window> window;
};
