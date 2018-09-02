//
//  CommandQueue.hpp
//  eowu-gl
//
//  Created by Nick Fagan on 9/1/18.
//

#pragma once

#include <functional>
#include <vector>
#include <shared_mutex>

namespace eowu {
  class CommandQueue;
  
  using CommandType = std::function<void()>;
  using CommandQueueType = std::vector<eowu::CommandType>;
}

class eowu::CommandQueue {
public:
  CommandQueue() = default;
  ~CommandQueue() = default;
  
  void Push(eowu::CommandType command);
  void Execute();
  
private:
  mutable std::shared_mutex mut;
  
  CommandQueueType commands;
};
