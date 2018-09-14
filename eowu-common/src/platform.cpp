//
//  platform.cpp
//  eowu-common
//
//  Created by Nick Fagan on 9/14/18.
//

#include "platform.hpp"

#ifdef EOWU_IS_WIN
const char* const eowu::path::slash = "\\";
#else
const char* const eowu::path::slash = "/";
#endif

#ifdef EOWU_IS_WIN
const char* const eowu::platform::name = "win";
#endif

#ifdef EOWU_IS_MAC
const char* const eowu::platform::name = "mac";
#endif

#ifdef EOWU_IS_LINUX
const char* const eowu::platform::name = "linux";
#endif
