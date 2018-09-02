//
//  test-resource-manager.hpp
//  eowu-gl
//
//  Created by Nick Fagan on 9/1/18.
//

#pragma once

#include <eowu-gl/eowu-gl.hpp>

void test_instantiation() {
  
  eowu::ResourceManager resource;
  
  auto res = resource.Create<eowu::Model>("example");
  
  if (!res) {
    std::cout << "ERROR: Invalid pointer associated with example" << std::endl;
  } else {
    std::cout << "OK: Created Model with id example" << std::endl;
  }
}

void test_retrieval() {
  eowu::ResourceManager resource;
  
  std::string id = "example";
  
  auto res = resource.Create<eowu::Model>(id);
  
  if (!resource.Has<eowu::Model>(id)) {
    std::cout << "ERROR: Missing model " << id << std::endl;
    return;
  } else {
    std::cout << "OK: Model " << id << " exists." << std::endl;
  }
  
  std::shared_ptr<eowu::Model> model;
  
  try {
    model = resource.Get<eowu::Model>(id);
    
    if (!model || model.get() != res.get()) {
      throw std::runtime_error("Retreived and created resources were different.");
    }
    
    std::cout << "OK: Retrieved model: " << id << std::endl;
  } catch (const std::exception &e) {
    std::cout << "ERROR: Retrieval failed with message: " << e.what() << std::endl;
  }
  
}

void test_resource_manager_run_all() {
  std::cout << "--------" << "resource manager" << std::endl;
  test_retrieval();
  test_instantiation();
}
