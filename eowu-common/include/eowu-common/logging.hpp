//
//  logging.hpp
//  eowu
//
//  Created by Nick Fagan on 8/28/18.
//

#pragma once

#include "config.hpp"

#ifdef EOWU_DEBUG
#define EOWU_LOG_WARN(value) std::cout << "WARN: " << value << std::endl
#else
#define EOWU_LOG_WARN(value)
#endif
