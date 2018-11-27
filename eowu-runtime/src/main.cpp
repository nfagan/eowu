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

namespace {
  std::string get_example_file(int argc, char** argv) {
    auto repo_root = eowu::fs::get_eowu_root_directory();
    
    std::string demo_folder;
    
    if (argc > 2) {
      demo_folder = argv[2];
    } else {
      demo_folder = "sound";
    }
    
    return eowu::fs::full_file({repo_root, "examples", demo_folder, "setup.lua"});
  }
}

int main(int argc, char** argv) {
  std::string file;
  
  if (argc < 2) {
    std::cout << std::endl << "Specify a file to run." << std::endl << std::endl;
    return 0;
  } else {
    file = argv[1];
    
    if (file == "--example") {
      file = get_example_file(argc, argv);
    }
  }
  
  return eowu::runtime::main(file);
}
