//
//  Types.hpp
//  eowu
//
//  Created by Nick Fagan on 9/7/18.
//

#pragma once

#include <eowu-common/types.hpp>
#include <vector>
#include <cstddef>

namespace eowu {
  namespace serialize {
    using ByteArrayType = std::vector<char>;
    
    namespace constants {
      static constexpr std::size_t flag_size = sizeof(eowu::u32);
      
      template<typename T>
      struct flag_type {};
      
      template<>
      struct flag_type<const double> {
        static constexpr eowu::u32 value = 1u << 1u;
      };
      
      template<>
      struct flag_type<double> {
        static constexpr eowu::u32 value = 1u << 1u;
      };
      
      template<>
      struct flag_type<bool> {
        static constexpr eowu::u32 value = 1u << 2u;
      };
      
      template<>
      struct flag_type<const char> {
        static constexpr eowu::u32 value = 1u << 3u;
      };
      
      template<>
      struct flag_type<std::string> {
        static constexpr eowu::u32 value = 1u << 3u;
      };
      
      template<>
      struct flag_type<const std::string> {
        static constexpr eowu::u32 value = 1u << 3u;
      };
      
      static constexpr eowu::u32 array = 1u << 4u;
      static constexpr eowu::u32 nd_array = 1u << 5u;
      static constexpr eowu::u32 aggregate = 1u << 6u;
      static constexpr eowu::u32 heterogeneous = 1u << 7u;
      static constexpr eowu::u32 unknown = 1u << 8u;
    }
  }
}
