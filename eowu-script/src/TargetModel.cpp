//
//  TargetModel.cpp
//  eowu-common
//
//  Created by Nick Fagan on 9/17/18.
//

#include "TargetModel.hpp"
#include <eowu-gl/eowu-gl.hpp>

eowu::TargetModel::TargetModel(std::shared_ptr<eowu::XYTarget> target_,
                               std::shared_ptr<eowu::Model> model_) :
target(target_), model(model_) {
  assert(target);
}

std::shared_ptr<eowu::Model> eowu::TargetModel::GetConfiguredModel() {
  const auto &target_trans = target->GetTransform();
  auto &model_trans = model->GetTransform();
  
  auto pos = target_trans.GetUnitsPosition();
  auto scl = target_trans.GetUnitsScale();
  auto pad = target->GetUnitsPadding();
  
  scl.x += pad.x;
  scl.y += pad.y;
  
  model_trans.SetPosition(pos);
  model_trans.SetScale(scl);
  model_trans.SetUnits(eowu::units::pixels);
  
  return model;
}
