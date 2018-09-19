//
//  RuntimeUtil.cpp
//  eowu-common
//
//  Created by Nick Fagan on 9/18/18.
//

#include "RuntimeUtil.hpp"
#include "Error.hpp"

std::string eowu::util::get_message_not_enough_inputs(const std::string &func, int minimum, int actual) {
  std::string min = std::to_string(minimum);
  std::string act = std::to_string(actual);
  
  return func + ": Not enough input arguments. Expected at least " + min + "; got " + act + ".";
}

std::string eowu::util::get_message_wrong_number_of_inputs(const std::string &func, int expected, int actual) {
  std::string expect = std::to_string(expected);
  std::string act = std::to_string(actual);
  
  return func + ": Wrong number of input arguments. Expected " + expect + "; got " + act + ".";
}

std::string eowu::util::get_message_wrong_input_type(const std::string &func,
                                                     const std::string &expected,
                                                     const std::string &actual) {
  
  return func + ": Incorrect input type. Expected '" + expected + "'; got '" + actual + "'.";
}

std::string eowu::util::get_message_nonexistent_resource(const std::string &func,
                                                         const std::string &kind,
                                                         const std::string &id) {
  
  return func + ": No '" + kind + "' exists with id '" + id + "'.";
}

std::string eowu::util::get_message_wrong_thread(const std::string &func,
                                                 const std::string &kind) {
  return func + ": This function cannot be called from the '" + kind + "' thread.";
}
