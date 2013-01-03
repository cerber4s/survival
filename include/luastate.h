#ifndef LUASTATE_H
#define LUASTATE_H

#include <lua.hpp>

class LuaState
{
public:
  LuaState();
  virtual ~LuaState();
  
  lua_State* GetLuaState() const;
  
  void RunScript(const char* filename);
  
private:
  lua_State* _luaState;

  static void RegisterClasses(lua_State* L);
  
};

#endif //LUASTATE_H