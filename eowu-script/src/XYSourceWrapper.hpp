//
//  XYSourceWrapper.hpp
//  eowu-common
//
//  Created by Nick Fagan on 12/22/18.
//

#pragma once

struct lua_State;

namespace eowu {
  class VectorWrapper;
  class XYSourceWrapper;
  class XYSource;
}

class eowu::XYSourceWrapper {
public:
  XYSourceWrapper(const eowu::XYSource *source);
  
  double GetX() const;
  double GetY() const;
  eowu::VectorWrapper GetPosition() const;
  
  static void CreateLuaSchema(lua_State *L);
private:
  const eowu::XYSource *source;
};
