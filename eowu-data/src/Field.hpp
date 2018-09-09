//
//  Field.hpp
//  eowu
//
//  Created by Nick Fagan on 9/7/18.
//

#pragma once

#include "Types.hpp"
#include "Serialize.hpp"
#include <string>
#include <vector>
#include <initializer_list>
#include "../../eowu-gl/deps/variant/include/mpark/variant.hpp"

namespace eowu {
  namespace data {
    template<typename ...T>
    struct Field {
      using value_type = mpark::variant<T..., std::vector<Field<T...>>>;
      
      std::string name;
      value_type value;
      
      Field() = default;
      template<typename K>
      Field(const std::string &name_, const K &value_) : name(name_), value(value_) {}
    };
    
    using Struct = Field<double, bool, std::string, std::vector<double>, std::vector<std::string>>;
    
    std::vector<eowu::data::Struct> make_nested(const eowu::data::Struct &other);
    std::vector<eowu::data::Struct> make_nested();
  }
  
  namespace serialize {
    template<typename ...T>
    void serialize(const eowu::data::Field<T...> &field, eowu::serialize::ByteArrayType &into, eowu::u32 flag = 0u);
    
    template<typename ...T>
    void serialize(const std::vector<eowu::data::Field<T...>> &fields, eowu::serialize::ByteArrayType &into, eowu::u32 flag = 0u);
  }
}

#include "Field.hh"
