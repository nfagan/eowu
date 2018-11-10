//
//  AudioSourceWrapper.cpp
//  eowu-common
//
//  Created by Nick Fagan on 11/10/18.
//

#include "AudioSourceWrapper.hpp"
#include "AudioSourceHandleWrapper.hpp"
#include "Constants.hpp"
#include "Lua.hpp"

eowu::AudioSourceWrapper::AudioSourceWrapper(std::shared_ptr<eowu::AudioSourceAggregate> aggregate,
                                             std::shared_ptr<eowu::AudioBufferSource> buffer) :
source_aggregate(aggregate), buffer_source(buffer) {
  //
}

eowu::AudioSourceHandleWrapper eowu::AudioSourceWrapper::Play() {
  auto handle = source_aggregate->Push(*buffer_source.get());
  eowu::AudioSourceHandleWrapper wrapper(handle);
  
  return wrapper;
}

double eowu::AudioSourceWrapper::SlotsRemaining() const {
  return double(source_aggregate->CountInactive());
}

double eowu::AudioSourceWrapper::Capacity() const {
  return double(source_aggregate->Capacity());
}

void eowu::AudioSourceWrapper::CreateLuaSchema(lua_State *L) {
  luabridge::getGlobalNamespace(L)
  .beginNamespace(eowu::constants::eowu_namespace)
  .beginClass<eowu::AudioSourceWrapper>("_AudioSource")
  .addFunction("Play", &eowu::AudioSourceWrapper::Play)
  .addFunction("OpenSlots", &eowu::AudioSourceWrapper::SlotsRemaining)
  .addFunction("Capacity", &eowu::AudioSourceWrapper::Capacity)
  .endClass()
  .endNamespace();
}
