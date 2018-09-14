//
//  DataInit.hpp
//  eowu-common
//
//  Created by Nick Fagan on 9/14/18.
//

#pragma once

#include "SetupResult.hpp"
#include <memory>

namespace eowu {
  namespace data {
    class Store;
    
    struct DataInit {
      std::shared_ptr<eowu::data::Store> task_data_store;
    };
    
    struct DataResult : eowu::SetupResult<DataInit> {};
    
    eowu::data::DataResult initialize_data_pipeline(const std::string &data_root_directory);
    
    eowu::SetupResult<std::string> create_date_container_directory(const std::string &data_root_directory);
    eowu::SetupResult<bool> check_root_directory_exists(const std::string &data_root_directory);
    eowu::SetupResult<std::shared_ptr<eowu::data::Store>> create_task_data_store(const std::string &data_subdirectory,
                                                                                 const std::string &data_filename);
    
    std::string get_date_container_directory();
  }
}
