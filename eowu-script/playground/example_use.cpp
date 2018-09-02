//
//  example_use.cpp
//  eowu-gl
//
//  Created by Nick Fagan on 8/31/18.
//

#include "example_use.hpp"

extern "C" {
 #include <lua.h>
 #include <lauxlib.h>
 #include <lualib.h>
}

#include <LuaBridge/LuaBridge.h>
#include <iostream>
#include <chrono>
#include <unordered_map>


namespace eowu {
  struct Color {
    Color(int r_, int b_, int g_) : r(r_), g(g_), b(b_) {}
    Color() : r(0), g(0), b(0) {}
    
    int r;
    int g;
    int b;
    
    void rgb(int r, int g, int b) {
      this->r = r;
      this->b = b;
      this->g = g;
    }
    
    void setR(int r) { this->r = r; }
    void setG(int g) { this->g = g; }
    void setB(int b) { this->b = b; }
    
    int getR() const { return r; }
  };
  
  class ExampleClass {
  public:
    ExampleClass(int param) : params(param) {};
    
    int getProperty() const {
      return params;
    }
    
    void setColor(Color color) {
      this->color = color;
    }
    
    Color getColor() const {
      return color;
    }
    
    void setProperty(int param) {
      params = param;
    }
    
    void printParam() {
      std::cout << std::endl;
      std::cout << "Param is: " << params << std::endl;
      std::cout << "Color is: " << color.r << ", " << color.g << ", " << color.b << std::endl;
    }
    
    static ExampleClass* Handle(const std::string &id) {
      static int id_ = 0;
      
      if (id != "some id") {
        return nullptr;
      }
      
      return new ExampleClass(id_++);
    }
    
  private:
    int params;
    
    Color color;
  };
}


std::unordered_map<std::string, luabridge::LuaRef> getKeyValueMap(const luabridge::LuaRef& table)
{
  using namespace luabridge;
  std::unordered_map<std::string, LuaRef> result;
  if (table.isNil()) { return result; }
  
  auto L = table.state();
  push(L, table); // push table
  
  lua_pushnil(L);  // push nil, so lua_next removes it from stack and puts (k, v) on stack
  while (lua_next(L, -2) != 0) { // -2, because we have table at -1
    if (lua_isstring(L, -2)) { // only store stuff with string keys
      std::cout << "KEY: " << lua_tostring(L, -2) << std::endl;
    }
    lua_pop(L, 1); // remove value, keep key for lua_next
  }
  
  lua_pop(L, 1); // pop table
  return result;
}

void test_func(int val) {
  std::cout << "TEST FUNCTION: " << val << std::endl;
}

eowu::Color test_get_color() {
  return eowu::Color{1, 2, 3};
}

int test_get_r() { return 0; }

void test_set_color(eowu::Color color) {
  //
}

void example_test() {
  using namespace luabridge;
  
  lua_State* L = luaL_newstate();
  
  luaL_openlibs(L);
  
  getGlobalNamespace(L)
  .beginNamespace("eowu")
    .beginClass<eowu::Color>("Color")
    .addProperty("r", &eowu::Color::getR, &eowu::Color::setR)
    .addFunction("rgb", &eowu::Color::rgb)
    .endClass()
    .beginClass<eowu::ExampleClass>("Stimulus")
    .addProperty("prop", &eowu::ExampleClass::getProperty, &eowu::ExampleClass::setProperty)
    .addProperty("color", &eowu::ExampleClass::getColor, &eowu::ExampleClass::setColor)
    .addFunction("print", &eowu::ExampleClass::printParam)
    .addStaticFunction("Handle", &eowu::ExampleClass::Handle)
    .endClass()
  .endNamespace();
  
  getGlobalNamespace(L)
    .beginNamespace("eowu")
    .beginNamespace("Target")
    .addProperty("color", test_get_color, test_set_color)
  .endNamespace()
  .endNamespace();
  
  
  int res = luaL_dofile(L, "/Users/Nick/repositories/cpp/eowu/build/eowu-script/Debug/script.lua");
  
  if (res != 0) {
    std::cout << lua_tostring(L, -1) << std::endl;
    return;
  }
  
  LuaRef func = getGlobal(L, "States");
  
  if (func.isNil()) {
    std::cout << "Missing state specifier!" << std::endl;
    return;
  }
  
  auto map = getKeyValueMap(func);
  
  LuaRef fixation = func["Fixation"];
  LuaRef entry = fixation["Entry"];
  LuaRef exit = fixation["Exit"];
  
//  map = getKeyValueMap(fixation);
  
  if (entry.isFunction()) {
    try {
      entry();
    } catch (const luabridge::LuaException &e) {
      std::cout << "Error: " << e.what() << std::endl;
      return;
    }
  }
  
  if (exit.isFunction()) {
    exit();
  }  
  
//  LuaRef entry = fixation["Entry"]();
//
//  auto t1 = std::chrono::high_resolution_clock::now();
//
//  for (std::size_t i = 0; i < 1000; i++) {
//    bool res = func(1, 0);
//  }
//
//  auto t2 = std::chrono::high_resolution_clock::now();
//
//  std::chrono::duration<double> time_span = std::chrono::duration_cast<std::chrono::duration<double>>(t2 - t1);
//
//  std::cout << "It took me " << time_span.count() << " seconds.";
}
