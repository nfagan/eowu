#pragma once

#include <eowu-gl/eowu-gl.hpp>
#include "test-util.hpp"
#include <iostream>
#include <vector>
#include <assert.h>

void test_vertex_instantiation();
void test_vertex_matching();
void test_attribute_matching();

void test_vertex_run_all() {
  std::cout << "--------" << "vertex" << std::endl;
  test_vertex_instantiation();
  test_vertex_matching();
  test_attribute_matching();
}

void test_vertex_instantiation() {
  eowu::Vertex vertex;
  
  auto values = get_values();
  
  eowu::Attribute attribute(eowu::attributes::position, values);
  
  vertex.AddAttribute(attribute);
  
  assert(vertex.HasAttribute(eowu::attributes::position));
  std::cout << "OK: Attribute " << eowu::attributes::position << " was present." << std::endl;
  
  assert(attribute.Size() == values.size());
  std::cout << "OK: Attribute was of size " << values.size() << std::endl;
}

void test_vertex_matching() {
  auto v1 = get_position_attribute();
  auto v2 = get_position_attribute();
  auto v3 = get_texture_attribute();
  
  assert(v1.Matches(v2));
  std::cout << "OK: Vertices with equal contents were equal." << std::endl;
  
  assert(!(v3.Matches(v2)));
  std::cout << "OK: Vertices with unequal contents were unequal." << std::endl;
}

void test_attribute_matching() {
  auto values = get_values();
  
  auto values2 = values;
  values2.push_back(0.3);
  
  eowu::Attribute attr1(eowu::attributes::position, values);
  eowu::Attribute attr2(eowu::attributes::position, values);
  eowu::Attribute attr3(eowu::attributes::position, values2);
  eowu::Attribute attr4("hi", values);
  
  assert(attr1.Matches(attr2));
  std::cout << "OK: Attributes with equal contents were equal." << std::endl;
  
  assert(!(attr3.Matches(attr2)));
  std::cout << "OK: Attributes with different values were not considered equal." << std::endl;
  
  assert(!(attr4.Matches(attr2)));
  std::cout << "OK: Attributes with different kinds were not considered equal." << std::endl;
  
}
