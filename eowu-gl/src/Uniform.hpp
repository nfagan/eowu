//
//  Uniform.hpp
//  eowu
//
//  Created by Nick Fagan on 8/28/18.
//

#pragma once

namespace eowu {
  template<typename T>
  struct Uniform;
}

template<typename T>
struct Uniform {
  const char *name;
  T value;
};
