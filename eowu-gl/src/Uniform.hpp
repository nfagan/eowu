//
//  Uniform.hpp
//  eowu
//
//  Created by Nick Fagan on 8/28/18.
//

#pragma once

#include "Texture.hpp"
#include <eowu-common/types.hpp>
#include <mpark/variant.hpp>
#include <glm/glm.hpp>
#include <string>

namespace eowu {
  class Uniform;
  
  using UniformVariantType = mpark::variant<eowu::Texture, bool, glm::vec3, glm::mat4, int, float>;
  
  template<typename UniformT>
  bool uniform_is_texture(const UniformT &value) {
    return mpark::visit([](auto &&arg) -> bool {
      using T = std::decay_t<decltype(arg)>;
      
      if constexpr (std::is_same_v<T, eowu::Texture>) {
        return true;
      } else {
        return false;
      }
    }, value);
  }
}

class eowu::Uniform {
public:
  Uniform();
  Uniform(const std::string &name, const eowu::UniformVariantType &value);
  
  ~Uniform() = default;
  
  void SetName(const std::string &name);
  void SetValue(const eowu::UniformVariantType &value);
  void SetContents(const std::string &name, const eowu::UniformVariantType &value);
  const std::string& GetName() const;
  
  void Set(unsigned int loc) const;
private:
  std::string name;
  eowu::UniformVariantType value;
  bool has_value;
};
