//
//  DataInit.cpp
//  eowu-common
//
//  Created by Nick Fagan on 9/14/18.
//

#include "DataInit.hpp"
#include <eowu-data.hpp>
#include <eowu-script/eowu-script.hpp>
#include <eowu-common/fs.hpp>
#include <iomanip>
#include <ctime>
#include <sstream>

#define EOWU_DATA_RESULT_EARLY_RETURN(from, to) \
  if (!from.status.success) { \
    to.status.message = from.status.message; \
    to.status.context = from.status.context; \
    return to; \
  }

std::string eowu::data::get_date_container_directory() {
  // https://stackoverflow.com/questions/16357999/current-date-and-time-as-string
  auto t = std::time(nullptr);
  auto tm = *std::localtime(&t);
  
  std::ostringstream oss;
  oss << std::put_time(&tm, "%m-%d-%Y %H-%M-%S");
  
  return oss.str();
}

eowu::SetupResult<std::string> eowu::data::create_date_container_directory(const std::string &data_root_directory) {
  eowu::SetupResult<std::string> result;
  
  auto date_container_directory = eowu::data::get_date_container_directory();
  auto full_path = eowu::fs::full_file({data_root_directory, date_container_directory});
  
  if (!eowu::fs::require_directory(full_path)) {
    result.status.message = "Failed to create date-container directory: '" + full_path + "'.";
    result.status.context = "data::create_date_container_directory";
    
    return result;
  }
  
  result.status.success = true;
  result.result = full_path;
  
  return result;
}

eowu::data::DataResult eowu::data::initialize_data_pipeline(const std::string &data_root_directory,
                                                            const eowu::schema::Save &schema) {
  
  eowu::data::DataResult result;
  std::string date_container_directory;
  
  bool any_saved_data = schema.save_state_data || schema.source_ids.size() > 0;  
  
  //  make sure root directory is valid
  if (any_saved_data) {
    auto data_root_result = eowu::data::check_root_directory_exists(data_root_directory);
    EOWU_DATA_RESULT_EARLY_RETURN(data_root_result, result);
    
    //  create the date container directory
    auto date_container_result = eowu::data::create_date_container_directory(data_root_directory);
    EOWU_DATA_RESULT_EARLY_RETURN(date_container_result, result);
    
    date_container_directory = date_container_result.result;
  }
  
  //  make sure task data file can be initialized successfully
  if (schema.save_state_data) {
    auto task_data_result = eowu::data::create_task_data_store(date_container_directory, "task.dat");
    EOWU_DATA_RESULT_EARLY_RETURN(task_data_result, result);
    
    result.result.task_data_store = task_data_result.result;
  } else {
    bool is_bypassed = true;
    result.result.task_data_store = std::make_shared<eowu::data::Store>(is_bypassed);
  }
  
  result.status.success = true;
  
  return result;
}

eowu::SetupResult<bool> eowu::data::check_root_directory_exists(const std::string &data_root_directory) {
  eowu::SetupResult<bool> result;
  
  if (!eowu::fs::directory_exists(data_root_directory)) {
    result.status.message = eowu::fs::nonexistent_directory_message(data_root_directory, "data");
    result.status.context = "data::check_root_directory_exists";
    return result;
  }
  
  result.status.success = true;
  result.result = true;
  
  return result;
}

eowu::SetupResult<std::shared_ptr<eowu::data::Store>> eowu::data::create_task_data_store(const std::string &data_subdirectory,
                                                                                         const std::string &data_filename) {
  eowu::SetupResult<std::shared_ptr<eowu::data::Store>> result;
  
  auto task_data_store = std::make_shared<eowu::data::Store>();
  
  const auto data_full_filename = eowu::fs::full_file({data_subdirectory, data_filename});
  
  try {
    task_data_store->Open(data_full_filename);
  } catch (const std::exception &e) {
    result.status.message = "Failed to open task data store file: '" + data_full_filename + "' with message: " + e.what();
    result.status.context = "data::create_task_data_store";
    
    return result;
  }
  
  result.status.success = true;
  result.result = task_data_store;
  
  return result;
}
