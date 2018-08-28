//
//  test-util.hpp
//  eowu
//
//  Created by Nick Fagan on 8/27/18.
//

#pragma once

namespace {
  std::vector<eowu::f32> get_values() {
    std::vector<eowu::f32> values;
    values.push_back(0.1);
    values.push_back(0.2);
    
    return values;
  }
  
  eowu::Attribute get_position_attribute() {
    auto values = get_values();
    return eowu::Attribute(eowu::attributes::position, values);
  }
  
  eowu::Attribute get_texture_attribute() {
    auto values = get_values();
    return eowu::Attribute(eowu::attributes::uv, values);
  }
}
