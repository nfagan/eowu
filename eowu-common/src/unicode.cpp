//
//  unicode.cpp
//  eowu-common
//
//  Created by Nick Fagan on 12/23/18.
//

#include "unicode.hpp"
#include <codecvt>

std::u32string eowu::unicode::u8_to_u32(const std::string &str) {
  std::wstring_convert<std::codecvt_utf8<char32_t>, char32_t> converter;
  return converter.from_bytes(str);
}

std::string eowu::unicode::u32_to_u8(const std::u32string &str) {
  std::wstring_convert<std::codecvt_utf8<char32_t>, char32_t> converter;
  return converter.to_bytes(str);
}

std::u16string eowu::unicode::u8_to_u16(const std::string &str) {
  //  https://en.cppreference.com/w/cpp/locale/codecvt_utf8_utf16
  return std::wstring_convert<std::codecvt_utf8_utf16<char16_t>, char16_t>{}.from_bytes(str);
}
