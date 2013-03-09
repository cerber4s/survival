#ifndef ENTITYMANAGER_H
#define ENTITYMANAGER_H

#include <boost/ptr_container/ptr_vector.hpp>

#include <luabind/luabind.hpp>

#include <map>
#include <queue>
#include <set>
#include <vector>

class Application;
class Entity;

class EntityManager
{
public:
  EntityManager(Application* application, int entityCount);
  virtual ~EntityManager();

  void Free(Entity* entity);

  bool TrySpawn(Entity** entity, const luabind::object& script);

  void ActivateSpawnedEntities();

  const std::set<Entity*>& GetActiveEntities() const;
  void GetActiveEntities(std::set<Entity*>& activeEntities) const;

  Entity* GetEntityById(int entityId) const;
  Entity* GetEntityByName(const std::string& name) const;
  const std::set<Entity*>& GetEntitiesByType(const std::string& type) const;

private:
  std::vector<Entity*> _entities;

  std::queue<Entity*> _availableEntities;
  std::set<Entity*> _spawnedEntities;
  std::set<Entity*> _activeEntities;
  
  std::map<int, Entity*> _entityByIdMap;
  std::map<std::string, Entity*> _entityByNameMap;
  std::map<std::string, std::set<Entity*>> _entitiesByTypeMap; 

  void EnsureEntityNameIsNotRegistered(const std::string& name);

  /*
  std::vector<Entity*> _entityPool;

  std::vector<Entity*> _entities;
  std::map<int, Entity*> _entityMap;
  std::map<std::string, Entity*> _entityNamesMap;
  std::map<std::string, std::set<Entity*>> _entityTypesMap; 

  std::vector<Entity*> _spawnedEntities;
  */

};

#endif