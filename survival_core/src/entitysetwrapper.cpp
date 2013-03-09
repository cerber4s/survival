#include "entitysetwrapper.h"

#include <luabind/luabind.hpp>
#include <luabind/iterator_policy.hpp>

#include "entity.h"

EntitySetWrapper::EntitySetWrapper(const std::set<Entity*>& entities) 
  : _entities(entities) 
{
}

EntitySetWrapper::EntitySetWrapper(const EntitySetWrapper& other) 
  : _entities(other.GetEntities()) 
{
}

EntitySetWrapper::~EntitySetWrapper() 
{
}

EntitySetWrapper& EntitySetWrapper::operator=(const EntitySetWrapper& other)
{
  if (this != &other)
  {
    _entities = other.GetEntities();
  }
  return *this;
}

const std::set<Entity*>& EntitySetWrapper::GetEntities() const
{
  return _entities;
}

void EntitySetWrapper::RegisterWithLua(lua_State* L)
{
  luabind::module(L)
    [
      luabind::class_<EntitySetWrapper>("entity_set_wrapper")
        .property("entities", &EntitySetWrapper::GetEntities, luabind::return_stl_iterator)
    ];
}

