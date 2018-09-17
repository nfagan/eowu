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
  int width, height, n_components;
  
  //  Correctly orient texture.
  stbi_set_flip_vertically_on_load(true);
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
  
  unsigned int texture_id;
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
  
  texture.SetWidth(width);
  texture.SetHeight(height);
  texture.SetId(texture_id);
  
  return texture;  
}
