//
//  TextureLoader.cpp
//  eowu-gl
//
//  Created by Nick Fagan on 8/29/18.
//

#define STB_IMAGE_IMPLEMENTATION

#include "TextureLoader.hpp"
#include "Error.hpp"
#include <stb_image.h>
#include <glad/glad.h>

eowu::Texture eowu::load::image(const std::string &filename) {
  eowu::s32 width, height, n_components;
  
  unsigned char *data = stbi_load(filename.c_str(), &width, &height, &n_components, 0);
  
  if (!data) {
    throw eowu::TextureLoadError("Failed to load file: " + filename);
  }
  
  GLenum format;
  
  switch (n_components) {
    case 1:
      format = GL_RED;
      break;
    case 3:
      format = GL_RGB;
      break;
    case 4:
      format = GL_RGBA;
      break;
    default:
      throw eowu::TextureLoadError("Unsupported texture format in file: " + filename);
  }
  
  eowu::u32 texture_id;
  glGenTextures(1, &texture_id);
  
  glBindTexture(GL_TEXTURE_2D, texture_id);
  glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
  glGenerateMipmap(GL_TEXTURE_2D);
  
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  
  stbi_image_free(data);
  
  eowu::Texture texture;
  
  texture.width = width;
  texture.height = height;
  texture.id = texture_id;
  texture.is_empty = false;
  
  return texture;  
}
