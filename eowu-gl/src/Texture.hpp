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
  ~Texture() = default;
  
  void SetId(unsigned int id);
  void SetWidth(unsigned int width);
  void SetHeight(unsigned int height);
  void SetIndex(unsigned int index);
  
  void Bind() const;
  void Dispose() const;
  
  unsigned int GetIndex() const;
  unsigned int GetWidth() const;
  unsigned int GetHeight() const;
  unsigned int GetId() const;
  
private:
  bool is_empty;
  
  unsigned int id;
  unsigned int width;
  unsigned int height;
  unsigned int index;
};
