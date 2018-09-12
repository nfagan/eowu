//
//  Variables.hpp
//  eowu
//
//  Created by Nick Fagan on 9/11/18.
//

#pragma once

#include <eowu-data.hpp>
#include <unordered_map>
#include <string>

namespace eowu {
  namespace schema {
    
    struct Variables {
      std::unordered_map<std::string, eowu::data::Commitable> mapping;
    };
  }
}

