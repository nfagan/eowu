//
//  TargetModel.hpp
//  eowu-common
//
//  Created by Nick Fagan on 9/17/18.
//

#pragma once

#include <memory>

namespace eowu {
  class XYTarget;
  class Model;
  
  class TargetModel;
}

class eowu::TargetModel {
  
public:
  TargetModel(std::shared_ptr<eowu::XYTarget> target,
              std::shared_ptr<eowu::Model> model);
  
  ~TargetModel() = default;
  
  std::shared_ptr<eowu::Model> GetConfiguredModel();
  
private:
  std::shared_ptr<eowu::XYTarget> target;
  std::shared_ptr<eowu::Model> model;
};
