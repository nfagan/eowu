//
//  unicode.hpp
//  eowu-common
//
//  Created by Nick Fagan on 12/23/18.
//

#pragma once

#include <string>
#include <locale>

namespace eowu {
  namespace unicode {
    std::u32string u8_to_u32(const std::string &str);
    std::u16string u8_to_u16(const std::string &str);
    std::string u32_to_u8(const std::u32string &str);
    std::string u16_to_u8(const std::u16string &str);
  }
}
