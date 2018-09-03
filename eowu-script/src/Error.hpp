//
//  Error.hpp
//  eowu-script
//
//  Created by Nick Fagan on 9/1/18.
//

#pragma once

#include <eowu-common/error.hpp>

namespace eowu {
  EOWU_GENERIC_ERROR(InvalidModelError);
  
  EOWU_GENERIC_ERROR(ScriptParseError);
  
  EOWU_GENERIC_ERROR(LuaError);
}
