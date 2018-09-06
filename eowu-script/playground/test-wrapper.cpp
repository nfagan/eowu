//
//  test-wrapper.cpp
//  eowu-script
//
//  Created by Nick Fagan on 9/1/18.
//

#include "test-wrapper.hpp"
#include "Lua.hpp"
#include <eowu-script/eowu-script.hpp>
#include <eowu-gl/eowu-gl.hpp>
#include <memory>

class Resources {
private:
  Resources() {
    resource_manager = std::make_shared<eowu::ResourceManager>();
    context_manager = std::make_shared<eowu::ContextManager>();
    renderer = std::make_shared<eowu::Renderer>(context_manager);
    texture_manager = std::make_shared<eowu::TextureManager>();
  }
  
public:
  std::shared_ptr<eowu::ResourceManager> resource_manager;
  std::shared_ptr<eowu::Renderer> renderer;
  std::shared_ptr<eowu::ContextManager> context_manager;
  std::shared_ptr<eowu::TextureManager> texture_manager;
  
public:
  static Resources* GetInstance() {
    static Resources* instance = nullptr;
    
    if (instance == nullptr) {
      instance = new Resources();
    }
    
    return instance;
  }
};

namespace detail {
 
  eowu::ModelWrapper create_model(const std::string &id) {
    using namespace eowu;
    
    auto instance = Resources::GetInstance();
    auto rsrc = instance->resource_manager;
    auto render = instance->renderer;
    auto mat = rsrc->Create<Material>(id);
    
    auto model = rsrc->Create<Model>(id, nullptr, mat);
    
    return ModelWrapper(model, render, instance->texture_manager);
  }
  
  eowu::ModelWrapper get_model(const std::string &id) {
    using namespace eowu;
    
    auto instance = Resources::GetInstance();
    auto rsrc = instance->resource_manager;
    auto render = instance->renderer;
    auto model = rsrc->Get<Model>(id);
    
    return ModelWrapper(model, render, instance->texture_manager);
  }
}

void test_vector_property_setting() {
  using namespace luabridge;
  using namespace eowu;
  
  lua_State* L = luaL_newstate();
  luaL_openlibs(L);
  
  VectorWrapper::CreateLuaSchema(L);
  ModelWrapper::CreateLuaSchema(L);
  
  getGlobalNamespace(L)
  .beginNamespace("eowu")
  .addFunction("CreateModel", detail::create_model)
  .addFunction("Model", detail::get_model)
  .endNamespace();
  
  util::run_lua_test_script(L, util::get_lua_test_script_directory() + "test-position-setting.lua");
}

void test_vector_setting()  {
  using namespace luabridge;
  using namespace eowu;
  
  lua_State* L = luaL_newstate();
  luaL_openlibs(L);
  VectorWrapper::CreateLuaSchema(L);
  util::insert_vector(L);
  
  util::run_lua_test_script(L, util::get_lua_test_script_directory() + "test-vector-setting.lua");
  
  
}

void test_wrapper_run_all() {
  test_vector_setting();
  test_vector_property_setting();
}
