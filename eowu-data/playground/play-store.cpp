//
//  play-store.cpp
//  eowu-common
//
//  Created by Nick Fagan on 9/8/18.
//

#include "play-store.hpp"
#include "test-util.hpp"
#include <eowu-data.hpp>

void eowu::data::play_store() {
  eowu::data::Store store;
  eowu::data::Struct s1;
  
  s1.name = "hello";
  s1.value = std::vector<double>{1.0, 2.0, 3.0, 4.0, 2.022};
  
  auto s2 = s1;
  auto s3 = s2;
  
  auto container = eowu::data::make_nested();
  auto s4 = s1;
  s4.name = "another";
  
  container.push_back(s1);
  container.push_back(s4);
  
  s1.value = container;
  
  auto file = eowu::test::util::get_test_data_dir() + "tmp.dat";
  
  eowu::serialize::ByteArrayType into;
//  eowu::serialize::serialize(s2, into);
//  eowu::serialize::serialize(s2, into);
  eowu::serialize::serialize(s1, into);
  eowu::serialize::serialize(1.0, into);
  eowu::serialize::serialize(std::vector<double>{1.0, 2.0, 4.0}, into);
  eowu::serialize::serialize(std::vector<std::string>{"hi", "another", "sup"}, into);
  
  store.Open(file);
  store.Write(into);
  store.Close();
}
