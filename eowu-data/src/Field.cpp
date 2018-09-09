//
//  Field.cpp
//  eowu-common
//
//  Created by Nick Fagan on 9/8/18.
//

#include "Field.hpp"

std::vector<eowu::data::Struct> eowu::data::make_nested(const eowu::data::Struct &other) {
  return std::vector<eowu::data::Struct>{other};
}

std::vector<eowu::data::Struct> eowu::data::make_nested() {
  return std::vector<eowu::data::Struct>();
}


