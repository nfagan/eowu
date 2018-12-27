//
//  TextModelWrapper.hpp
//  eowu-common
//
//  Created by Nick Fagan on 12/25/18.
//

#pragma once

#include <memory>

struct lua_State;

namespace eowu {
  class TextModelWrapper;
  class FontFace;
  class TextModel;
  class Renderer;
  class WindowContainerMap;
}

class eowu::TextModelWrapper {
public:
  TextModelWrapper(std::shared_ptr<eowu::TextModel> model,
                   std::shared_ptr<eowu::FontFace> font_face,
                   std::shared_ptr<eowu::Renderer> renderer,
                   std::shared_ptr<eowu::WindowContainerMap> window_container);
  
  ~TextModelWrapper() = default;
  
  int SetColor(lua_State *L);
  int SetPositionVector(lua_State *L);
  void SetText(const std::string &text);
  void SetUnits(const std::string &units);
  void SetScale(double scale);
  void SetIsCentered(bool value);
  
  int Draw(lua_State *L);
  
  static void CreateLuaSchema(lua_State *L);
private:
  std::shared_ptr<eowu::TextModel> text_model;
  std::shared_ptr<eowu::FontFace> font_face;
  std::shared_ptr<eowu::Renderer> renderer;
  std::shared_ptr<eowu::WindowContainerMap> window_container;  
};
