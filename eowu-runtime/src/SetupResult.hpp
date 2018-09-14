//
//  SetupResult.hpp
//  eowu-common
//
//  Created by Nick Fagan on 9/14/18.
//

#pragma once

#include "SetupStatus.hpp"

namespace eowu {
  template<typename T>
  struct SetupResult;
}

template<typename T>
struct eowu::SetupResult {
  eowu::SetupStatus status;
  T result;
};
