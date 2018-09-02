//
//  Targets.hpp
//  eowu
//
//  Created by Nick Fagan on 9/2/18.
//

#include <unordered_map>
#include <string>
#include <glm/glm.hpp>

namespace eowu {
  namespace schema {
    struct Target {
      std::string kind;
      std::string source_id;
      std::vector<double> padding;
    };
    
    struct Targets {
      std::unordered_map<std::string, Target> targets;
    };
  }
}
