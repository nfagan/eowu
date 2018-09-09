//
//  play-serialize.cpp
//  eowu-common
//
//  Created by Nick Fagan on 9/7/18.
//

#include "play-serialize.hpp"
#include <eowu-data.hpp>
#include <assert.h>
#include <iostream>

void eowu::serialize::play_serialize() {
//  using variant_t = mpark::variant<int, double>;
//  std::vector<variant_t> res;
//  
//  res.push_back(1);
//  res.push_back(true);
//  
//  using field_t = eowu::data::Field<double, bool, std::vector<double>>;
//  using nested_field_t = eowu::data::Field<double, bool, field_t>;
//  
//  field_t f1("sup", std::vector<double>({1.0, 2.0, 3.0}));
//  nested_field_t f2("sup", f1);
//  
//  eowu::data::Struct test_field;
//  eowu::data::Struct another_field;
//  
//  test_field.value = eowu::data::make_nested(test_field);
//  test_field.value = eowu::data::make_nested(eowu::data::Struct{"first", std::vector<double>()});
//  test_field.value = eowu::data::make_nested(test_field);
//  
//  eowu::data::Struct s1;
//  
//  eowu::serialize::ByteArrayType into;
//  
////  eowu::serialize::serialize(f2, into);
//  
//  std::vector<nested_field_t> res2;
//  res2.push_back(f2);
//  res2.push_back(nested_field_t("a", true));
//  
////  eowu::serialize::serialize(f1, into);
////  eowu::serialize::serialize(nested, into);
//  
//  double num = 1.02;
//  
//  eowu::serialize::serialize(num, into);
//  
//  eowu::u32 type;
//  double val;
//  
//  std::memcpy(&type, &into[0], sizeof(eowu::u32));
//  std::memcpy(&val, &into[sizeof(eowu::u32)], sizeof(double));
//  
//  std::cout << type << std::endl;
//  assert(val == num);
//  
//  std::cout << "OK" << std::endl;
//  std::cout << "OK" << std::endl;
//  
//  eowu::serialize::serialize(test_field, into);
}
