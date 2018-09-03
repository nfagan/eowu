//
//  GLResourceWrapper.hpp
//  eowu-common
//
//  Created by Nick Fagan on 9/2/18.
//

#pragma once

#include <memory>

//auto instance = Resources::GetInstance();
//auto rsrc = instance->resource_manager;
//auto queue = instance->command_queue;

namespace eowu {
  class ResourceManager;
  class CommandQueue;
  
  class GLResourceWrapper;
}

class eowu::GLResourceWrapper {
public:
  GLResourceWrapper(std::shared_ptr<eowu::ResourceManager> gl_resource_manager, std::shared_ptr<eowu::CommandQueue> cmd);
};
