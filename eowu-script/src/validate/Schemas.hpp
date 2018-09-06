//
//  ValidationSchemas.hpp
//  eowu-common
//
//  Created by Nick Fagan on 9/5/18.
//

#pragma once

#include <vector>
#include <string>

#define EOWU_VALIDATION_NAMESPACE_BEGIN namespace eowu { namespace schema { namespace validate {
#define EOWU_VALIDATION_NAMESPACE_END } } }

#include <vector>

EOWU_VALIDATION_NAMESPACE_BEGIN

struct Geometry {
  std::vector<std::string> builtin_ids;
};

struct Target {
  std::vector<std::string> source_ids;
  std::vector<std::string> kinds;
};

struct Stimulus {
  std::vector<std::string> texture_ids;
  std::vector<std::string> geometry_ids;
  std::vector<std::string> target_ids;
  std::vector<std::string> units;
};

struct Setup {
  eowu::schema::validate::Geometry geometry;
  eowu::schema::validate::Target target;
  eowu::schema::validate::Stimulus stimulus;
};

EOWU_VALIDATION_NAMESPACE_END
