//
//  test2.cpp
//  eowu-script
//
//  Created by Nick Fagan on 9/1/18.
//

extern "C" {
#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>
}

#include <LuaBridge/LuaBridge.h>

#include "test2.hpp"

struct Color {
  int r;
  int g;
  int b;
  
  Color(int r_, int g_, int b_) : r(r_), g(g_), b(b_) {};
  
  void set_r(int r_) { r = r_; }
  int get_r() const { return r; }
};

Color example_getter(const std::string &id) {
  return Color{0, 255, 0};
}

void test2() {
  using namespace luabridge;
  
  lua_State* L = luaL_newstate();
  
  luaL_openlibs(L);
  
  getGlobalNamespace(L)
  .beginNamespace("eowu")
  .beginClass<Color>("Color")
  .addProperty("r", &Color::get_r, &Color::set_r)
  .endClass()
  .addFunction("Stimulus", example_getter)
  .endNamespace();
  
  
  int res = luaL_dofile(L, "/Users/Nick/repositories/cpp/eowu/build/eowu-script/Debug/test2.lua");  
}
