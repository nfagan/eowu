#include "test-wrapper.hpp"
#include "test-parser.hpp"
#include "test-parse-util.hpp"

int main(int argc, char** argv) {
  
  test_parse_util_run_all();
  test_wrapper_run_all();
  test_parser_run_all();
  
  return 0;
}
