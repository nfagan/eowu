//
//  debug.hpp
//  eowu
//
//  Created by Nick Fagan on 8/28/18.
//

#pragma once

namespace eowu {
  namespace shaders {
    namespace debug {
      
      //
      //  VERTEX
      //
      
      static const char *vertex = R"(
      #version 330 core
      
      layout (location = 0) in vec3 position;
      layout (location = 1) in vec2 uv;
      
      void main() {
        gl_Position = vec4(position, 1.0f);
      })";
      
      //
      //  FRAGMENT
      //
      
      static const char *fragment = R"(
      #version 330 core
      
      out vec4 FragColor;
      
      void main() {
        FragColor = vec4(1.0f, 0.1f, 0.0f, 1.0f);
      })";
    }
  }
}
