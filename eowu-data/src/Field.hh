//
//  Field.hh
//  eowu
//
//  Created by Nick Fagan on 9/7/18.
//

#pragma once

template<typename ...T>
void eowu::serialize::serialize(const eowu::data::Field<T...> &field, eowu::serialize::ByteArrayType &into, eowu::u32 flag) {
  auto aggregate_flag = flag | eowu::serialize::constants::aggregate;
  
  eowu::serialize::serialize(field.name, into, aggregate_flag);
  mpark::visit([&](auto &&arg) { eowu::serialize::serialize(arg, into, flag); }, field.value);
}

template<typename ...T>
void eowu::serialize::serialize(const std::vector<eowu::data::Field<T...>> &fields, eowu::serialize::ByteArrayType &into, eowu::u32 flag) {
  
  eowu::serialize::priv::nd_array(fields.size(), into, flag | eowu::serialize::constants::aggregate);
  
  for (const auto &field : fields) {
    eowu::serialize::serialize(field, into, flag);
  }
}
