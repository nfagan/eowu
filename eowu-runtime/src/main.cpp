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
  
//  if (argc < 2) {
//    std::cout << "Specify a file." << std::endl;
//    return 0;
//  }
  
//  std::string file(argv[1]);
  
  std::string file = "/Users/Nick/repositories/cpp/eowu/eowu-runtime/test/script/brief-valid-schema.lua";
  
  return runtime.Main(file);
}
