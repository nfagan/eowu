//
//  RuntimeUtil.hpp
//  eowu-common
//
//  Created by Nick Fagan on 9/18/18.
//

#pragma once

#include <string>

namespace eowu {
  namespace util {
    
    std::string get_message_wrong_number_of_inputs(const std::string &func,
                                                   int expected,
                                                   int actual);
    
    std::string get_message_not_enough_inputs(const std::string &func,
                                              int minimum,
                                              int actual);
    
    std::string get_message_wrong_input_type(const std::string &func,
                                             const std::string &expected,
                                             const std::string &actual);
    
    std::string get_message_nonexistent_resource(const std::string &func,
                                                 const std::string &kind,
                                                 const std::string &id);
  }
}
