#include "steeringbehaviors.h"

#include <luabind/luabind.hpp>

#include "entity.h"

SteeringBehaviors::SteeringBehaviors(Entity* entity) 
  : _entity(entity)
{
}

SteeringBehaviors::~SteeringBehaviors()
{
}

Vector2d SteeringBehaviors::Calculate()
{
  if (_script.is_valid())
  {
    return luabind::call_function<Vector2d>(_script["calculate"], _script, _entity);
  }
  
  throw std::runtime_error("steering behaviors script is not valid");
}

luabind::object& SteeringBehaviors::GetScript()
{
  return _script;
}

void SteeringBehaviors::SetScript(const luabind::object& script)
{
  _script = script;
}

void SteeringBehaviors::RegisterWithLua(lua_State* L)
{
  luabind::module(L)
  [
    luabind::class_<SteeringBehaviors>("steering_behaviors")
      .def("calculate", &SteeringBehaviors::Calculate)
      .property("script", &SteeringBehaviors::GetScript, &SteeringBehaviors::SetScript)
  ];
}