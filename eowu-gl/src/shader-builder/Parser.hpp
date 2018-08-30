//
//  Builder.hpp
//  eowu
//
//  Created by Nick Fagan on 8/29/18.
//

#pragma once

#include "Types.hpp"
#include <string>

namespace eowu {
  namespace parser {
    std::string fragment(const eowu::schema::Fragment &schema);
    std::string vertex(const eowu::schema::Vertex &schema);
  }
}
