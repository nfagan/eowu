//
//  Units.hpp
//  eowu
//
//  Created by Nick Fagan on 8/28/18.
//

#pragma once

#include <eowu-common/types.hpp>
#include <vector>
#include <string>

namespace eowu {
  namespace units {
    static constexpr unsigned int normalized = 0;
    static constexpr unsigned int pixels = 1;
    
    std::vector<std::string> get_string_unit_labels();
    unsigned int get_units_from_string_label(const std::string &unit_id);
  }
}
