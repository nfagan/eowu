//
//  Setup.hpp
//  eowu-script
//
//  Created by Nick Fagan on 9/3/18.
//

#pragma once

#include "Windows.hpp"
#include "Textures.hpp"
#include "Geometry.hpp"
#include "Stimuli.hpp"
#include "State.hpp"
#include "Targets.hpp"
#include "Save.hpp"
#include "Source.hpp"
#include "Paths.hpp"
#include "Variables.hpp"

namespace eowu {
  namespace schema {
    struct EntryScript {
      luabridge::LuaRef entry_table;
      
      EntryScript(lua_State *L) : entry_table(L) {}
    };
    
    struct Setup {
      eowu::schema::Windows windows;
      eowu::schema::Textures textures;
      eowu::schema::Geometry geometry;
      eowu::schema::States states;
      eowu::schema::Targets targets;
      eowu::schema::Stimuli stimuli;
      eowu::schema::Sources sources;
      eowu::schema::Paths paths;
      eowu::schema::Variables variables;
      eowu::schema::Save save;
    };
  }
}
