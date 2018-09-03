//
//  RenderFunction.cpp
//  eowu-common
//
//  Created by Nick Fagan on 9/2/18.
//

#include "RenderFunction.hpp"

eowu::RenderFunction::RenderFunction() {
  SetOnNextFrame([]() {});
}

void eowu::RenderFunction::Call() const {
  std::unique_lock<std::mutex> lock(mut);
  on_next_frame();
}

void eowu::RenderFunction::SetOnNextFrame(const RenderFunctionCallbackType &cb) {
  std::unique_lock<std::mutex> lock(mut);
  on_next_frame = cb;
}
