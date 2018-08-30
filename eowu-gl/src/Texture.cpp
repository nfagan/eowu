//
//  Texture.cpp
//  eowu-gl
//
//  Created by Nick Fagan on 8/29/18.
//

#include "Texture.hpp"
#include <glad/glad.h>

eowu::Texture::Texture() {
  is_empty = true;
  id = 0;
  width = 0;
  height = 0;
  index = 0;
}

eowu::Texture::~Texture() {
  if (!is_empty) {
    glDeleteTextures(1, &id);
  }
}
