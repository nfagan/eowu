#include "test-wrapper.hpp"
#include "test-parser.hpp"
#include "test-parse-util.hpp"
//#include "test-gl-script.hpp"
#include "test-mutex-script.hpp"
#include "test-state-wrapper.hpp"
#include "test-gl-wrapper.hpp"
#include "test-multiple-lua-states.hpp"

int main(int argc, char** argv) {
  
  test_states::test_states_main();
  gltest::test_gl_wrapper_run_all();
//  test_gl_script_run_all();
  test_parse_util_run_all();
  test_wrapper_run_all();
  test_parser_run_all();
  
  return 0;
}
