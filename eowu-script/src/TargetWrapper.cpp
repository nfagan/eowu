//
//  TargetWrapper.cpp
//  eowu-common
//
//  Created by Nick Fagan on 9/17/18.
//

#include "TargetWrapper.hpp"
#include "LuaContext.hpp"
#include "Constants.hpp"
#include "Lua.hpp"
#include "Error.hpp"
#include "parser/ParseUtil.hpp"
#include <eowu-gl/eowu-gl.hpp>

eowu::TargetWrapper::TargetWrapper(std::shared_ptr<eowu::LuaContext> lua_context_,
                                   std::shared_ptr<eowu::XYTarget> target_,
                                   std::shared_ptr<eowu::Model> model_,
                                   std::shared_ptr<eowu::Renderer> renderer_,
                                   std::shared_ptr<eowu::WindowContainerMap> window_container_) :
lua_context(lua_context_), target(target_), model(model_),
renderer(renderer_), window_container(window_container_),
on_entry(lua_context_->GetState()), on_exit(lua_context_->GetState()), is_hidden(false) {
  configure_target_callbacks();
}

void eowu::TargetWrapper::Reset() {
  const auto &noop = eowu::LuaFunction::get_no_op(lua_context->GetState());
  
  on_entry.Set(noop);
  on_exit.Set(noop);
  
  target->Reset();
}

void eowu::TargetWrapper::SetOnEntry(luabridge::LuaRef func) {
  if (!func.isFunction()) {
    throw eowu::LuaError("Argument to 'Entry' must be a function.");
  }
  
  on_entry.Set(func);
}

void eowu::TargetWrapper::SetOnExit(luabridge::LuaRef func) {
  if (!func.isFunction()) {
    throw eowu::LuaError("Argument to 'Exit' must be a function.");
  }
  
  on_exit.Set(func);
}

void eowu::TargetWrapper::Hide() {
  is_hidden = true;
}

void eowu::TargetWrapper::Show() {
  is_hidden = false;
}

int eowu::TargetWrapper::Draw(lua_State *L) {
  //  @TODO: Refactor to share code between this and ModelWrapper.
  
  if (is_hidden) {
    return 0;
  }
  
  int inputs = lua_gettop(L);
  
  std::vector<std::string> window_ids;
  
  //  1, because user data is first input
  if (inputs == 1) {
    window_ids = window_container->Keys();
  } else {
    if (inputs != 2) {
      const std::string arg = std::to_string(inputs-1);
      throw eowu::LuaError("Invalid arguments to 'Draw()': Expected 1 or 0 inputs, got " + arg);
    }
    
    window_ids = eowu::parser::get_string_vector_from_state(L, -1);
  }
  
  //  set-up transforms as appropriate.
  configure_model();
  
  for (const auto &win_id : window_ids) {
    renderer->Queue(*model.get(), window_container->Get(win_id));
  }
  
  return 0;
}

void eowu::TargetWrapper::configure_target_callbacks() {
  target->SetOnEntry([&](auto *target) -> void {
    lua_context->Call(on_entry);
  });
  
  target->SetOnExit([&](auto *target) -> void {
    lua_context->Call(on_exit);
  });
}

void eowu::TargetWrapper::configure_model() {
  const auto &target_trans = target->GetTransform();
  auto &model_trans = model->GetTransform();
  
  auto pos = target_trans.GetUnitsPosition();
  auto scl = target_trans.GetUnitsScale();
  auto pad = target->GetUnitsPadding();
  
  scl.x += pad.x;
  scl.y += pad.y;
  
  model_trans.SetPosition(pos);
  model_trans.SetScale(scl);
  model_trans.SetUnits(eowu::units::pixels);
}

void eowu::TargetWrapper::CreateLuaSchema(lua_State *L) {
  luabridge::getGlobalNamespace(L)
  .beginNamespace(eowu::constants::eowu_namespace)
  .beginClass<eowu::TargetWrapper>("_Target")
  .addCFunction("Draw", &eowu::TargetWrapper::Draw)
  .addFunction("Entry", &eowu::TargetWrapper::SetOnEntry)
  .addFunction("Exit", &eowu::TargetWrapper::SetOnExit)
  .addFunction("Reset", &eowu::TargetWrapper::Reset)
  .endClass()
  .endNamespace();
}
