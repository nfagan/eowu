#include "test-wrapper.hpp"
#include "test-parser.hpp"
#include "test-parse-util.hpp"
//#include "test-gl-script.hpp"
#include "test-mutex-script.hpp"
#include "test-state-wrapper.hpp"

int main(int argc, char** argv) {
  
  test::test_state_wrapper_run_all();
  test_mutex_script();
//  test_gl_script_run_all();
  test_parse_util_run_all();
  test_wrapper_run_all();
  test_parser_run_all();
  
  return 0;
}
