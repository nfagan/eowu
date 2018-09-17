//
//  Save.hpp
//  eowu
//
//  Created by Nick Fagan on 9/16/18.
//

#pragma once

#include <unordered_set>
#include <string>

namespace eowu {
  namespace schema {
    struct Save {
      bool save_state_data;
      
      bool provided_source_ids;
      std::unordered_set<std::string> source_ids;
      
      Save() : save_state_data(false), provided_source_ids(false) {}
    };
  }
}
