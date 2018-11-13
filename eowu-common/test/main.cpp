//
//  main.cpp
//  eowu-common
//
//  Created by Nick Fagan on 9/14/18.
//

#include "test-path.hpp"
#include "test-directory-contents.hpp"

int main(int argc, char **argv) {
  eowu::test::test_path();
  eowu::test::test_fullfile();
  eowu::test::test_directory_contents();
  
  return 0;
}
