//
//  LuaContext.hh
//  eowu
//
//  Created by Nick Fagan on 9/9/18.
//

#pragma once

template<typename ...Args>
void eowu::LuaContext::Call(eowu::LuaFunction &other, Args... args) const {
  std::unique_lock<std::mutex> lock(mut);
  other.Call(args...);
}
