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
//    glDeleteTextures(1, &id);
  }
}

void eowu::Texture::Bind() const {
  glActiveTexture(GL_TEXTURE0 + index);
  glBindTexture(GL_TEXTURE_2D, id);
}

void eowu::Texture::SetId(eowu::u32 id) {
  this->id = id;
  is_empty = false;
}

void eowu::Texture::SetIndex(eowu::u32 index) {
  this->index = index;
}

void eowu::Texture::SetWidth(eowu::u32 width) {
  this->width = width;
}

void eowu::Texture::SetHeight(eowu::u32 height) {
  this->height = height;
}

eowu::u32 eowu::Texture::GetIndex() const {
  return index;
}

eowu::u32 eowu::Texture::GetHeight() const {
  return height;
}

eowu::u32 eowu::Texture::GetWidth() const {
  return width;
}
