//
//  RenderFunction.hpp
//  eowu-common
//
//  Created by Nick Fagan on 9/2/18.
//

#pragma once

#include <mutex>
#include <functional>
#include <atomic>

namespace eowu {
  class RenderFunction;
  using RenderFunctionCallbackType = std::function<void(void)>;
}

class eowu::RenderFunction {
public:
  RenderFunction();
  ~RenderFunction() = default;
  
  void SetOnNextFrame(const RenderFunctionCallbackType &cb);
  void Call() const;
  
private:
  mutable std::mutex mut;
  
  RenderFunctionCallbackType on_next_frame;
};
