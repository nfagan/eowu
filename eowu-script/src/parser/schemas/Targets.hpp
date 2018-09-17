//
//  Targets.hpp
//  eowu
//
//  Created by Nick Fagan on 9/2/18.
//

#pragma once

#include <unordered_map>
#include <string>
#include <glm/glm.hpp>

namespace eowu {
  namespace schema {
    struct Target {
      std::string target_id;
      std::string type;
      std::string source_id;
      std::string model_id;
      std::string window_id;
      std::vector<double> padding;
      
      bool provided_model_id;
      
      Target() : padding({0, 0, 0}), provided_model_id(false) {
        //
      }
    };
    
    struct Targets {
      std::unordered_map<std::string, Target> targets;
    };
  }
}
