//
//  ValidationSchemas.hpp
//  eowu-common
//
//  Created by Nick Fagan on 9/5/18.
//

#pragma once

#include <string>
#include <unordered_set>

#define EOWU_VALIDATION_NAMESPACE_BEGIN namespace eowu { namespace schema { namespace validate {
#define EOWU_VALIDATION_NAMESPACE_END } } }

EOWU_VALIDATION_NAMESPACE_BEGIN

struct Geometry {
  std::unordered_set<std::string> builtin_ids;
};

struct Target {
  std::unordered_set<std::string> source_ids;
  std::unordered_set<std::string> window_ids;
  std::unordered_set<std::string> model_ids;
  std::unordered_set<std::string> kinds;
};

struct Stimulus {
  std::unordered_set<std::string> texture_ids;
  std::unordered_set<std::string> geometry_ids;
  std::unordered_set<std::string> units;
};

struct Source {
  std::unordered_set<std::string> source_types;
  std::unordered_set<std::string> window_ids;
};

struct Setup {
  eowu::schema::validate::Geometry geometry;
  eowu::schema::validate::Target target;
  eowu::schema::validate::Stimulus stimulus;
  eowu::schema::validate::Source source;
};

EOWU_VALIDATION_NAMESPACE_END
