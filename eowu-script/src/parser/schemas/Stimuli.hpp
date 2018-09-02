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
      std::string geometry_id;
      std::string texture_id;
      std::vector<double> color;
      std::vector<std::string> target_ids;
      std::string units;
      std::vector<double> size;
      std::vector<double> position;
    };
    
    struct Stimuli {
      std::unordered_map<std::string, eowu::schema::Stimulus> stimuli;
    };
  }
}
