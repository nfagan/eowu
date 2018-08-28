//
//  test-shader.hpp
//  eowu
//
//  Created by Nick Fagan on 8/28/18.
//

#pragma once

#include "test-util.hpp"
#include <eowu-gl/eowu-gl.hpp>
#include <iostream>

namespace source {
  static const char *fragment = R"(
#version 330 core
  
  out vec4 FragColor;
  in vec2 TextureCoordinates;
  
  uniform bool albedo_use_texture;
  uniform sampler2D albedo_value_sampler2d;
  uniform vec3 albedo_value_vec3;
  
  void main()
  {
    if (albedo_use_texture) {
      FragColor = vec4(texture(albedo_value_sampler2d, TextureCoordinates).rgb, 1.0f);
    } else {
      FragColor = vec4(albedo_value_vec3, 1.0f);
    }
  })";
  
  static const char *vertex = R"(
#version 330 core
  layout (location = 0) in vec3 position;
  layout (location = 1) in vec2 tex_coords;
  
  uniform mat4 model;
  uniform mat4 view;
  uniform mat4 projection;
  
  out vec2 TextureCoordinates;
  
  void main()
  {
    TextureCoordinates = tex_coords;
    gl_Position = projection * view * model * vec4(position, 1.0f);
  })";
}

eowu::Shader get_frag_shader() {
  eowu::Shader frag;
  frag.Create(source::fragment, eowu::shader_types::fragment);
  return frag;
}

eowu::Shader get_vert_shader() {
  eowu::Shader vert;
  vert.Create(source::vertex, eowu::shader_types::vertex);
  return vert;
}

void test_shader_compile();
void test_program_link();

void test_program_link() {
  auto frag = get_frag_shader();
  auto vert = get_vert_shader();
  
  eowu::Program prog;
  
  std::vector<eowu::Shader*> shaders;
  
  shaders.push_back(&frag);
  shaders.push_back(&vert);
  
  try {
    prog.Link(shaders);
    std::cout << "OK: Linking valid program succeeded." << std::endl;
  } catch (const std::exception &e) {
    std::cout << "ERROR: Linking valid program failed with message: " << e.what() << std::endl;
  }
  
  eowu::Program prog2;
  eowu::Shader vert1;
  
  std::vector<eowu::Shader*> uninit_shaders;
  uninit_shaders.push_back(&vert1);
  
  try {
    prog2.Link(uninit_shaders);
    std::cout << "ERROR: Program successfully linked uninitialized shaders" << std::endl;
    
    prog2.Link(shaders);
    std::cout << "ERROR: Program successfully linked uninitialized shaders" << std::endl;
  } catch (const std::exception& e) {
    std::cout << "OK: Program linking uninitialized shaders raised error: " << e.what() << std::endl;
  }
  
}

void test_shader_compile() {
  auto* window = init_glfw();
  
  if (!window) {
    std::cout << "ERROR: GLFW failed to initialize" << std::endl;
    return;
  }
  
  eowu::Shader frag;
  eowu::Shader vert;
  
  try {
    frag.Create(source::fragment, eowu::shader_types::fragment);
    std::cout << "OK: Valid fragment source successfully compiled." << std::endl;
  } catch (const std::exception& e) {
    std::cout << "ERROR: Compiling valid fragment source failed with message: " << e.what() << std::endl;
  }
  
  try {
    vert.Create(source::vertex, eowu::shader_types::vertex);
    std::cout << "OK: Valid vertex source successfully compiled." << std::endl;
  } catch (const std::exception& e) {
    std::cout << "ERROR: Compiling valid vertex source failed with message: " << e.what() << std::endl;
  }
  
  assert(frag.IsCreated());
  assert(vert.IsCreated());
  
  frag.Dispose();
  vert.Dispose();
  
  assert(!frag.IsCreated());
  assert(!vert.IsCreated());
  
  glfwWindowShouldClose(window);
}

void test_shader_run_all() {
  test_shader_compile();
  test_program_link();
}
