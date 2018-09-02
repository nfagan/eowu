#include "test-wrapper.hpp"
#include "test-gl-script.hpp"

int main(int argc, char** argv) {
  
  test_gl_script_run_all();
  test_wrapper_run_all();
  
  return 0;
}
