//
//  LuaFunction.hh
//  eowu
//
//  Created by Nick Fagan on 9/9/18.
//

#pragma once

template<typename ...Args>
void eowu::LuaFunction::Call(Args... args) {
  std::unique_lock<std::recursive_mutex> lock(mut);
  
  //  it's important that did_call come before function_reference()
  //  in case the lua call results in an error -- otherwise, the task
  //  thread might get stuck in an infinite loop
  did_call = true;
  function_reference(args...);
}
