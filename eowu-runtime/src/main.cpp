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
    std::cout << "Enter a path to a file to run." << std::endl << std::endl;
    return 0;
  } else {
    file = argv[1];
  }
  
  return runtime.Main(file);
}
