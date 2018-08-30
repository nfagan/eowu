//
//  Aggregates.cpp
//  eowu-gl
//
//  Created by Nick Fagan on 8/29/18.
//

#include "Aggregates.hpp"
#include "Components.hpp"
#include "Types.hpp"

eowu::aggregates::ComponentAggregateType eowu::aggregates::declare_mvp() {
  ComponentAggregateType res;
  
  res.push_back(eowu::components::declare_uniform("model", eowu::glsl::mat4));
  res.push_back(eowu::components::declare_uniform("view", eowu::glsl::mat4));
  res.push_back(eowu::components::declare_uniform("projection", eowu::glsl::mat4));
  
  return res;
}
