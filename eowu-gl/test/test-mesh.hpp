//
//  test-mesh.hpp
//  eowu
//
//  Created by Nick Fagan on 8/27/18.
//

#pragma once

#include "test-util.hpp"
#include <eowu-gl/eowu-gl.hpp>
#include <iostream>
#include <vector>
#include <assert.h>

void test_mesh_add_inconsistent_vertices();
void test_draw();

void test_mesh_run_all() {
  test_mesh_add_inconsistent_vertices();
  test_draw();
}

void test_draw() {
  auto* window = init_glfw();
  
  eowu::Program prog = eowu::program_factory::make_debug();
  eowu::Mesh mesh;
  eowu::mesh_factory::make_quad(mesh);
  
  while (!glfwWindowShouldClose(window))
  {
    glClear(GL_COLOR_BUFFER_BIT);
    
    prog.Bind();
    mesh.Draw();
    
    glfwSwapBuffers(window);
    glfwPollEvents();
  }
}

void test_mesh_add_inconsistent_vertices() {
  
  eowu::Mesh mesh;
  eowu::Vertex v1;
  eowu::Vertex v2;
  eowu::Vertex v3;
  
  const auto pos = get_position_attribute();
  const auto uv = get_texture_attribute();
  
  v1.AddAttribute(pos);
  v2.AddAttribute(pos);
  v3.AddAttribute(pos);
  v3.AddAttribute(uv);
  
  try {
    mesh.AddVertex(v1);
    std::cout << "OK: Single vertex added successfully." << std::endl;
    
    mesh.AddVertex(v2);
    std::cout << "OK: Second matching vertex added successfully" << std::endl;
  } catch (const std::exception &e) {
    throw e;
  }
  
  try {
    mesh.AddVertex(v3);
  } catch (const std::exception &e) {
    std::cout << "OK: Adding inconsistent vertex raised error: " << e.what() << std::endl;
  }
}
