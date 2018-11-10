//
//  AudioSourceHandleWrapper.cpp
//  eowu-common
//
//  Created by Nick Fagan on 11/10/18.
//

#include "AudioSourceHandleWrapper.hpp"
#include "Constants.hpp"
#include "Lua.hpp"

eowu::AudioSourceHandleWrapper::AudioSourceHandleWrapper(std::shared_ptr<eowu::AudioSourceHandle> handle) :
source_handle(handle) {
  //
}

void eowu::AudioSourceHandleWrapper::Stop() {
  source_handle->Stop();
}

void eowu::AudioSourceHandleWrapper::CreateLuaSchema(lua_State *L) {
  luabridge::getGlobalNamespace(L)
  .beginNamespace(eowu::constants::eowu_namespace)
  .beginClass<eowu::AudioSourceHandleWrapper>("_AudioSourceHandle")
  .addFunction("Stop", &eowu::AudioSourceHandleWrapper::Stop)
  .endClass()
  .endNamespace();
}
