//
//  CommandQueue.cpp
//  eowu-gl
//
//  Created by Nick Fagan on 9/1/18.
//

#include "CommandQueue.hpp"
#include <mutex>

void eowu::CommandQueue::Push(eowu::CommandType command) {
  std::unique_lock<std::shared_mutex> lock(mut);
  commands.push_back(command);
}

void eowu::CommandQueue::Execute() {
  std::unique_lock<std::shared_mutex> lock(mut);
  
  for (const auto &cmd : commands) {
    cmd();
  }
  
  commands.clear();
}
