//
//  Variant.hpp
//  eowu
//
//  Created by Nick Fagan on 8/29/18.
//

#pragma once

#include <mpark/variant.hpp>

namespace eowu {
  // https://stackoverflow.com/questions/47203255/convert-stdvariant-to-another-stdvariant-with-super-set-of-types
  
  namespace {
    template <class... Args>
    struct variant_cast_proxy {
      mpark::variant<Args...> v;
      
      template <class... ToArgs>
      operator mpark::variant<ToArgs...>() const {
        return mpark::visit([](auto&& arg) -> mpark::variant<ToArgs...> { return arg; }, v);
      }
    };
  }
  
  template <class... Args>
  auto variant_cast(const mpark::variant<Args...>& v) -> variant_cast_proxy<Args...> {
    return {v};
  }
}
