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

namespace eowu {
  namespace schema {
    struct Setup {
      eowu::schema::Windows windows;
      eowu::schema::Textures textures;
      eowu::schema::Geometry geometry;
      eowu::schema::States states;
      eowu::schema::Targets targets;
      eowu::schema::Stimuli stimuli;
    };
  }
}
