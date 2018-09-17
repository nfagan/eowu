//
//  Units.cpp
//  eowu-common
//
//  Created by Nick Fagan on 9/6/18.
//

#include "Units.hpp"
#include "Error.hpp"

std::unordered_set<std::string> eowu::units::get_string_unit_labels() {
  return { "normalized", "pixels", "mixed" };
}

unsigned int eowu::units::get_units_from_string_label(const std::string &unit_id) {
  if (unit_id == "normalized") {
    return eowu::units::normalized;
  } else if (unit_id == "pixels") {
    return eowu::units::pixels;
  } else if (unit_id == "mixed") {
    return eowu::units::mixed;
  } else {
    throw eowu::UnrecognizedLabelError("No units matched '" + unit_id + "'");
  }
}
