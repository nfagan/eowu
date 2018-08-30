//
//  TextureLoader.hpp
//  eowu-gl
//
//  Created by Nick Fagan on 8/29/18.
//

#pragma once

#include "Texture.hpp"
#include <memory>
#include <string>
#include <eowu-common/types.hpp>

namespace eowu {
  namespace load {
    eowu::Texture image(const std::string &filename);
  }
}
