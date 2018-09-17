//
//  SetupValidator.hpp
//  eowu-common
//
//  Created by Nick Fagan on 9/5/18.
//

#include "Schemas.hpp"
#include "Util.hpp"
#include "../parser/Schemas.hpp"
#include <unordered_set>
#include <unordered_map>
#include <string>

namespace eowu {
  namespace validate {
    eowu::ValidationResult setup(const eowu::schema::Setup &schema,
                                 const eowu::schema::validate::Setup &validation);
    
    eowu::ValidationResult geometry(const eowu::schema::Geometry &schema,
                                    const eowu::schema::validate::Geometry &validation);
    
    eowu::ValidationResult stimuli(const eowu::schema::Stimuli &schema,
                                   const eowu::schema::validate::Stimulus &validation);
    eowu::ValidationResult stimulus(const eowu::schema::Stimulus &schema,
                                    const eowu::schema::validate::Stimulus &validation);
    
    eowu::ValidationResult target(const eowu::schema::Target &schema,
                                  const eowu::schema::validate::Target &validation);
    eowu::ValidationResult targets(const eowu::schema::Targets &schema,
                                   const eowu::schema::validate::Target &validation);
    
    eowu::ValidationResult source(const eowu::schema::Source &schema,
                                  const eowu::schema::validate::Source &validation);
    eowu::ValidationResult sources(const eowu::schema::Sources &schema,
                                   const eowu::schema::validate::Source &validation);
    
    eowu::ValidationResult states(const eowu::schema::States &schema);
    
    eowu::ValidationResult io(const eowu::schema::Paths &paths_schema, const eowu::schema::Save &save_schema);
    
    eowu::schema::validate::Setup make_setup(const eowu::schema::Setup &schema);
    eowu::schema::validate::Target make_target(const eowu::schema::Setup &schema);
    eowu::schema::validate::Stimulus make_stimulus(const eowu::schema::Setup &schema);
    eowu::schema::validate::Geometry make_geometry(const eowu::schema::Setup &schema);
    eowu::schema::validate::Source make_source(const eowu::schema::Setup &schema);
    
    template<typename T>
    eowu::ValidationResult map_check_duplicates(std::unordered_set<std::string> &visited,
                                                const std::unordered_map<std::string, T> &to_check,
                                                const std::string &kind,
                                                const std::string &context);
  }
}

#include "SetupValidator.hh"
