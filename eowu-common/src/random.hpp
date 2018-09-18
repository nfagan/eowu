//
//  random.hpp
//  eowu-common
//
//  Created by Nick Fagan on 9/17/18.
//

#pragma once

#include <string>
#include <cstddef>

namespace eowu {
  namespace random {
    std::string get_random_string(std::size_t N);
  }
}
