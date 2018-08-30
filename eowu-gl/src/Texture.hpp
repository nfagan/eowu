//
//  Texture.hpp
//  eowu-gl
//
//  Created by Nick Fagan on 8/29/18.
//

#pragma once

#include <eowu-common/types.hpp>

namespace eowu {
  struct Texture;
}

struct eowu::Texture {
  Texture();
  ~Texture();
  
  bool is_empty;
  
  eowu::u32 id;
  eowu::u32 width;
  eowu::u32 height;
  eowu::u32 index;
};
