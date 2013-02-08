#ifndef STEERINGBEHAVIORS_H
#define STEERINGBEHAVIORS_H

#include <lua.hpp>
#include <luabind/luabind.hpp>

#include "vector2d.h"

class Entity;

class SteeringBehaviors
{
public:
  SteeringBehaviors(Entity* entity);
  virtual ~SteeringBehaviors();
  
  Vector2d Calculate();

  luabind::object& GetScript();
  void SetScript(const luabind::object& script);
  
  static void RegisterWithLua(lua_State* L);
  
private:
  Entity* _entity;
  
  luabind::object _script;
  
};

#endif