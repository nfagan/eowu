//
//  Texture.hpp
//  eowu-gl
//
//  Created by Nick Fagan on 8/29/18.
//

#pragma once

#include <eowu-common/types.hpp>

namespace eowu {
  class Texture;
}

class eowu::Texture {
public:
  Texture();
  ~Texture();
  
  void SetId(eowu::u32 id);
  void SetWidth(eowu::u32 width);
  void SetHeight(eowu::u32 height);
  void SetIndex(eowu::u32 index);
  
  void Bind() const;
  
  eowu::u32 GetIndex() const;
  eowu::u32 GetWidth() const;
  eowu::u32 GetHeight() const;
  
private:
  bool is_empty;
  
  eowu::u32 id;
  eowu::u32 width;
  eowu::u32 height;
  eowu::u32 index;
};
