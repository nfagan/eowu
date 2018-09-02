//
//  SetupParser.hpp
//  eowu-common
//
//  Created by Nick Fagan on 9/2/18.
//

#pragma once

#include "Schemas.hpp"
#include "ParseResult.hpp"

namespace luabridge {
  class LuaRef;
}

namespace eowu {
  namespace parser {
    ParseResult<eowu::schema::Windows> windows(const luabridge::LuaRef &table);
    ParseResult<eowu::schema::Geometry> geometry(const luabridge::LuaRef &table);
    ParseResult<eowu::schema::Textures> textures(const luabridge::LuaRef &table);
    
    ParseResult<eowu::schema::Stimulus> stimulus(const luabridge::LuaRef &table);
    ParseResult<eowu::schema::Stimuli> stimuli(const luabridge::LuaRef &table);
    
    ParseResult<eowu::schema::Target> target(const luabridge::LuaRef &table);
    ParseResult<eowu::schema::Targets> targets(const luabridge::LuaRef &table);
    
    ParseResult<eowu::schema::State> state(const luabridge::LuaRef &table);
    ParseResult<eowu::schema::States> states(const luabridge::LuaRef &table);
  }
}
