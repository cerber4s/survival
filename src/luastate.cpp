#include "luastate.h"

#include <luabind/luabind.hpp>
#include <boost/format.hpp>
#include <exception>

#include "application.h"
#include "color.h"
#include "entity.h"
#include "matrix3.h"
#include "rendersystem.h"
#include "vector2d.h"

LuaState::LuaState()
{
  _luaState = luaL_newstate();
  luaL_openlibs(_luaState);
  
  luabind::open(_luaState);
  
  LuaState::RegisterClasses(_luaState);
}

LuaState::~LuaState()
{
  lua_close(_luaState);
  _luaState = nullptr;
}

lua_State* LuaState::GetLuaState() const
{
  return _luaState;
}

void LuaState::RunScript(const char* filename)
{
  if (int error = luaL_dofile(_luaState, filename))
  {
    const char* msg = lua_tolstring(_luaState, -1, 0);
    throw std::runtime_error((boost::format("failed to run script '%s' [%d]: %s") % filename % error % msg).str());
  }
}

void LuaState::RegisterClasses(lua_State* L)
{
  Application::RegisterWithLua(L);
  Color::RegisterWithLua(L);
  Entity::RegisterWithLua(L);
  Matrix3::RegisterWithLua(L);
  RenderSystem::RegisterWithLua(L);
  Vector2d::RegisterWithLua(L);
}
