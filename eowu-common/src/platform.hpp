//
//  platform.hpp
//  eowu
//
//  Created by Nick Fagan on 9/14/18.
//

#pragma once

#ifdef __APPLE__
#define EOWU_IS_MAC
#endif

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32)
#define EOWU_IS_WIN
#endif

#ifdef linux
#define EOWU_IS_LINUX
#endif

namespace eowu {
  namespace fs {
    extern const char* const slash;
  }
  
  namespace platform {
    extern const char* const name;
  }
}

