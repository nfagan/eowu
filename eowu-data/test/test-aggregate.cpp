//
//  test-aggregate.cpp
//  eowu-common
//
//  Created by Nick Fagan on 9/8/18.
//

#include "test-aggregate.hpp"
#include <eowu-data.hpp>
#include <eowu-common/test.hpp>

void eowu::test::test_aggregate() {
  eowu::data::Struct s1;
  
  s1.name = "hello";
  s1.value = std::vector<double>{1.0, 2.0, 3.0, 4.0};
  
  auto s2 = s1;
  
  s2.name = "hi";
  s2.value = eowu::data::make_nested(s1);
  
  eowu::serialize::ByteArrayType into;
  
  eowu::serialize::serialize(s2, into);
  
  eowu::u32 type;
  
  std::memcpy(&type, into.data(), sizeof(eowu::u32));
  
  constexpr auto type_flag = eowu::serialize::constants::flag_type<const char>::value;
  constexpr auto aggregate_flag = eowu::serialize::constants::aggregate;
  constexpr auto array_flag = eowu::serialize::constants::array;
  
  auto expected_flag = type_flag | aggregate_flag | array_flag;
  
  EOWU_ASSERT_TRUE(expected_flag == type, "Aggregate flag and type flag were present.", "Unexpected aggregate + type flag.");
}
