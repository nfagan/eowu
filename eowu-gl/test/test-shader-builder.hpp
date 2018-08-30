//
//  test-shader-builder.hpp
//  eowu
//
//  Created by Nick Fagan on 8/30/18.
//

#pragma once

#include <eowu-gl/eowu-gl.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include <vector>
#include <assert.h>

void check_fragment() {
  eowu::Mesh mesh;
  eowu::Material material;
  
  eowu::mesh_factory::make_quad(&mesh);
  eowu::Shader fragment;
  
  const auto frag_src = eowu::builder::fragment(mesh, material);
  
  try {
    fragment.Create(frag_src.c_str(), eowu::shader_types::fragment);
    
    std::cout << "OK: Built source was successfully compiled." << std::endl;
  } catch (const std::exception &e) {
    std::cout << "ERROR: Compiling built-source failed with message: " << e.what() << std::endl;
    std::cout << frag_src << std::endl;
  }
}

void check_fragment_texture() {
  eowu::Mesh mesh;
  eowu::Material material;
  
  eowu::mesh_factory::make_quad(&mesh);
  eowu::Shader fragment;
  eowu::Texture texture;
  
  material.SetFaceColor(texture);
  
  const auto frag_src = eowu::builder::fragment(mesh, material);
  
  try {
    fragment.Create(frag_src.c_str(), eowu::shader_types::fragment);
    
    std::cout << "OK: Built source was successfully compiled." << std::endl;
  } catch (const std::exception &e) {
    std::cout << "ERROR: Compiling built-source failed with message: " << e.what() << std::endl;
    std::cout << frag_src << std::endl;
  }
}

void check_vertex() {
  
  eowu::Mesh mesh;
  eowu::Material material;
  
  eowu::mesh_factory::make_quad(&mesh);
  eowu::Shader vertex;
  
  const auto vert_src = eowu::builder::vertex(mesh, material);
  
  try {
    vertex.Create(vert_src.c_str(), eowu::shader_types::vertex);
    
    std::cout << "OK: Built source was successfully compiled." << std::endl;
  } catch (const std::exception &e) {
    std::cout << "ERROR: Compiling built-source failed with message: " << e.what() << std::endl;
    std::cout << vert_src << std::endl;
  }
}

void test_shader_builder_run_all() {
  std::cout << "--------" << "shader builder" << std::endl;
  check_vertex();
  check_fragment();
  check_fragment_texture();
}
