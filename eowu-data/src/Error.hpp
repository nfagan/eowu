//
//  Error.hpp
//  eowu
//
//  Created by Nick Fagan on 9/8/18.
//

#pragma once

#include <eowu-common/error.hpp>

namespace eowu {
  EOWU_GENERIC_ERROR(AlreadyCommittedError);
  
  EOWU_GENERIC_ERROR(FileIOError);
}
