//
//  init.hpp
//  eowu-script
//
//  Created by Nick Fagan on 9/3/18.
//

#pragma once

#include <memory>
#include <unordered_map>
#include <string>

namespace eowu {
  class LuaContext;
  class LuaFunction;
  class StateWrapper;
  
  using StateWrapperMapType = std::unordered_map<std::string, std::shared_ptr<eowu::StateWrapper>>;
  using StateWrapperContainerType = std::unique_ptr<eowu::StateWrapperMapType>;
  using LuaFunctionMapType = std::unordered_map<std::string, LuaFunction>;
  using LuaFunctionContainerType = std::unique_ptr<LuaFunctionMapType>;
}
