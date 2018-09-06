#include "test-wrapper.hpp"
#include "test-parser.hpp"
#include "test-parse-util.hpp"
//#include "test-gl-script.hpp"
#include "test-mutex-script.hpp"
#include "test-state-wrapper.hpp"
#include "test-gl-wrapper.hpp"
#include "test-state-parse.hpp"
#include "test-multiple-lua-states.hpp"
#include "test-alternate-threaded-model.hpp"
#include "test-script-wrapper.hpp"
#include "test-entry-parse.hpp"

int main(int argc, char** argv) {
  test::entry::run_all();
  
  
//  gltest::test_gl_wrapper_run_all();
  script::test_gl_wrapper_run_all();
  
  test_state_parse_run_all();
//  test_states::test_states_main();
  test_parse_util_run_all();
  test_wrapper_run_all();
  test_parser_run_all();
  
  return 0;
}
