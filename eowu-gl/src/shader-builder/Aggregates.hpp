//
//  Aggregates.hpp
//  eowu-gl
//
//  Created by Nick Fagan on 8/29/18.
//

#pragma once

#include <string>
#include <vector>

namespace eowu {
  namespace aggregates {
    using ComponentAggregateType = std::vector<std::string>;
    
    ComponentAggregateType declare_mvp();
  }
}
