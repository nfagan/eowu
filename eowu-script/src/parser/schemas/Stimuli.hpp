//
//  Stimuli.hpp
//  eowu
//
//  Created by Nick Fagan on 9/2/18.
//

#pragma once

#include <unordered_map>
#include <string>
#include <vector>

namespace eowu {
  namespace schema {
    
    struct Stimulus {
      std::string stimulus_id;
      std::string geometry_id;
      std::string texture_id;
      bool provided_texture_id;
      std::vector<double> color;
      std::string units;
      std::vector<double> size;
      std::vector<double> position;
      std::vector<double> rotation;
      
      Stimulus() : provided_texture_id(false), size({0, 0, 0}), position({0, 0, 0}), rotation({0, 0, 0}) {
        //
      }
      
    };
    
    struct Stimuli {
      std::unordered_map<std::string, eowu::schema::Stimulus> stimuli;
    };
  }
}
