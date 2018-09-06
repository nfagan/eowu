#include "test-parser.hpp"
#include "test-parse-util.hpp"
#include "test-state-parse.hpp"
#include "test-entry-parse.hpp"
#include "test-window-parser.hpp"
#include "test-stimuli-parse.hpp"
#include "test-noop-insert.hpp"

int main(int argc, char** argv) {
  test::entry::run_all();
  test_window();
  test_stimuli_run_all();
  test_state_parse_run_all();
  test_parse_util_run_all();
  test_parser_run_all();
  eowu::test::test_noop_insert();
  
  return 0;
}
