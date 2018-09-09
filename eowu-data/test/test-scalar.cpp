//
//  test-scalar.cpp
//  eowu-common
//
//  Created by Nick Fagan on 9/8/18.
//

#include "test-scalar.hpp"
#include <eowu-data.hpp>
#include <eowu-common/test.hpp>

void eowu::test::test_serialize_number() {
  EOWU_PRINT_TEST_BEGIN("Serialize number");
  
  eowu::serialize::ByteArrayType into;
  
  double num = 1.0001;
  
  eowu::serialize::serialize(num, into);
  
  eowu::u32 type;
  double val;
  
  std::memcpy(&type, &into[0], sizeof(eowu::u32));
  std::memcpy(&val, &into[sizeof(eowu::u32)], sizeof(double));
  
  EOWU_ASSERT_TRUE(val == num, "Deserialized number equalled serialized number.",
                   "Deserialized number did not match serialized number.");
  
  auto expected_type = eowu::serialize::constants::flag_type<double>::value;
  
  EOWU_ASSERT_TRUE(expected_type == type, "Deserialized type (number) equalled serialized type.",
                   "Deserialized type (number) did not match serialized type.");
}
