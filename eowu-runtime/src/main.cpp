//
//  main.cpp
//  eowu-common
//
//  Created by Nick Fagan on 9/5/18.
//

#include "Runtime.hpp"
#include <string>
#include <iostream>

int main(int argc, char** argv) {
  eowu::Runtime runtime;
  
  std::string file;
  
  if (argc < 2) {
    file = "/Users/Nick/repositories/cpp/eowu/eowu-runtime/test/script/brief-valid-schema.lua";
  } else {
    file = argv[1];
  }
  
  return runtime.Main(file);
}
