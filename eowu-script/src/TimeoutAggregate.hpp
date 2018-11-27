//
//  TimeoutAggregate.hpp
//  eowu-common
//
//  Created by Nick Fagan on 11/17/18.
//

#pragma once

#include <eowu-common/LockedResource.hpp>
#include <unordered_map>
#include <string>
#include <memory>
#include <mutex>

namespace eowu {
  class TimeoutWrapper;
  class TimeoutHandle;
  
  struct TimeoutAggregateContents {
    std::shared_ptr<eowu::TimeoutWrapper> wrapper;
    std::shared_ptr<eowu::TimeoutHandle> handle;
  };
  
  using TimeoutAggregateMapType = eowu::LockedResource<std::unordered_map<std::string, eowu::TimeoutAggregateContents>, std::recursive_mutex>;
}
