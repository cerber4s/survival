#ifndef ENTITYSETWRAPPER_H
#define ENTITYSETWRAPPER_H

#include <lua.hpp>
#include <luabind/luabind.hpp>

#include <set>

class Entity;

class EntitySetWrapper
{
public:
  EntitySetWrapper(const std::set<Entity*>& entities);
  EntitySetWrapper(const EntitySetWrapper& other);
  virtual ~EntitySetWrapper();

  EntitySetWrapper& operator=(const EntitySetWrapper& other);
  
  const std::set<Entity*>& GetEntities() const;

  static void RegisterWithLua(lua_State* L);

private:
  std::set<Entity*> _entities;

};

#endif // ENTITYSETWRAPPER_H
