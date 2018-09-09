//
//  main.cpp
//  eowu-common
//
//  Created by Nick Fagan on 9/8/18.
//

#include "test-scalar.hpp"
#include "test-aggregate.hpp"

int main(int argc, char **argv) {
  eowu::test::test_serialize_number();
  eowu::test::test_aggregate();
  
  return 0;
}
