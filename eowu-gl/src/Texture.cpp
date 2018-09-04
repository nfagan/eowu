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

void eowu::Texture::Bind() const {
  glActiveTexture(GL_TEXTURE0 + index);
  glBindTexture(GL_TEXTURE_2D, id);
}

void eowu::Texture::Dispose() const {
  glDeleteTextures(1, &id);
}

void eowu::Texture::SetId(unsigned int id) {
  this->id = id;
  is_empty = false;
}

void eowu::Texture::SetIndex(unsigned int index) {
  this->index = index;
}

void eowu::Texture::SetWidth(unsigned int width) {
  this->width = width;
}

void eowu::Texture::SetHeight(unsigned int height) {
  this->height = height;
}

unsigned int eowu::Texture::GetIndex() const {
  return index;
}

unsigned int eowu::Texture::GetHeight() const {
  return height;
}

unsigned int eowu::Texture::GetWidth() const {
  return width;
}
