//
//  main.cpp
//  eowu-common
//
//  Created by Nick Fagan on 9/5/18.
//

#include "Runtime.hpp"
#include <eowu-common/fs.hpp>
#include <string>
#include <iostream>

int main(int argc, char** argv) {
  eowu::Runtime runtime;
  
  std::string file;
  
  if (argc < 2) {
    std::cout << std::endl << "Specify a file to run." << std::endl << std::endl;
    return 0;
  } else {
    file = argv[1];
    
    if (file == "--demo") {
      auto repo_root = eowu::fs::get_eowu_root_directory();
      file = eowu::fs::full_file({repo_root, "examples", "sound", "setup.lua"});
    }
  }
  
  return runtime.Main(file);
}
