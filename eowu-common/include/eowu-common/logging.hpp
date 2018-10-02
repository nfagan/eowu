//
//  logging.hpp
//  eowu
//
//  Created by Nick Fagan on 8/28/18.
//

#pragma once

#include "config.hpp"
#include "debug.hpp"

#ifdef EOWU_DEBUG
#include <iostream>
#endif

#ifdef EOWU_DEBUG
#define EOWU_LOG_WARN(value) std::cout << eowu::debug::colors::yellow << "WARN: " << eowu::debug::colors::dflt << value << std::endl
#else
#define EOWU_LOG_WARN(value)
#endif

#ifdef EOWU_DEBUG
#define EOWU_LOG_INFO(value) std::cout << eowu::debug::colors::green << "INFO: " << eowu::debug::colors::dflt << value << std::endl
#else
#define EOWU_LOG_INFO(value)
#endif
