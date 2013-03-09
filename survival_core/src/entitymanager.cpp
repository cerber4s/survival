#include "entitymanager.h"

#include <boost/foreach.hpp>
#include <boost/format.hpp>

#include <cassert>

#include "entity.h"

EntityManager::EntityManager(Application* appication, int entityCount)
{  
  assert( entityCount >= 0 );

  for (int i = 0; i < entityCount; i++)
  {
    Entity* entity = new Entity(appication);

    _entities.push_back(entity);
    _availableEntities.push(entity);
  }
}

EntityManager::~EntityManager()
{
  BOOST_FOREACH(Entity* entity, _entities)
  {
    entity->Reset();
    delete entity;
  }
}

void EntityManager::Free(Entity* entity)
{
  // todo assert that the entity is active or spawned?

  size_t count;
  
  count = _activeEntities.erase(entity);
  assert( count == 1);

  count = _entityByIdMap.erase(entity->GetId());
  assert( count == 1 );

  count = _entityByNameMap.erase(entity->GetName());
  assert( count == 1 );

  std::map<std::string, std::set<Entity*>>::iterator i = _entitiesByTypeMap.find(entity->GetType());
  if (i == _entitiesByTypeMap.end())
  {
    throw std::runtime_error((boost::format("type '%s' not found in entity types map") % entity->GetType()).str());
  }

  count = (*i).second.erase(entity);
  assert( count == 1 );
}

bool EntityManager::TrySpawn(Entity** entity, const luabind::object& script)
{
 if (!_availableEntities.empty())
 {
   *entity = _availableEntities.front();
   _availableEntities.pop();

   (*entity)->Initialize(script);

   _spawnedEntities.insert(*entity);

   return true;
 }

 *entity = nullptr;
 return false;
}

void EntityManager::ActivateSpawnedEntities()
{
  if (!_spawnedEntities.empty())
  {
    BOOST_FOREACH(Entity* entity, _spawnedEntities)
    {
      EnsureEntityNameIsNotRegistered(entity->GetName());

      _entityByIdMap[entity->GetId()] = entity;
      _entityByNameMap[entity->GetName()] = entity;

      std::map<std::string, std::set<Entity*>>::iterator i = _entitiesByTypeMap.find(entity->GetType());
      if (i == _entitiesByTypeMap.end())
      {
        _entitiesByTypeMap[entity->GetType()] = std::set<Entity*>();
        _entitiesByTypeMap[entity->GetType()].insert(entity);
      }
      else
      {
        (*i).second.insert(entity);
      }
    }

    std::copy(_spawnedEntities.begin(), _spawnedEntities.end(), std::inserter(_activeEntities, _activeEntities.begin()));
    _spawnedEntities.clear();
  }
}

const std::set<Entity*>& EntityManager::GetActiveEntities() const
{
  return _activeEntities;
}

void EntityManager::GetActiveEntities(std::set<Entity*>& activeEntities) const
{
  std::copy(_activeEntities.begin(), _activeEntities.end(), std::inserter(activeEntities, activeEntities.begin()));
}

Entity* EntityManager::GetEntityById(int entityId) const
{
  std::map<int, Entity*>::const_iterator i = _entityByIdMap.find(entityId);  

  Entity* entity = nullptr;
  if (i != _entityByIdMap.end())
  {
    entity = (*i).second;
  }

  return entity;
}

Entity* EntityManager::GetEntityByName(const std::string& name) const
{
  std::map<std::string, Entity*>::const_iterator i = _entityByNameMap.find(name);
  return (i != _entityByNameMap.end()) ? (*i).second : nullptr;
}

const std::set<Entity*>& EntityManager::GetEntitiesByType(const std::string& type) const
{
  static std::set<Entity*> emptyResult;

  std::map<std::string, std::set<Entity*>>::const_iterator i = _entitiesByTypeMap.find(type);
  return (i != _entitiesByTypeMap.end()) ? (*i).second : emptyResult;
}

//void EntityManager::ClearInactiveEntities()
//{
//  auto i = _entities.begin();
//  while (i != _entities.end())
//  {
//    Entity* entity = *i; 
//    if (!entity->IsActive())
//    {
//      _entityPool.push_back(entity);
//
//      _entityMap.erase(entity->GetId());
//
//      _entityNamesMap.erase(entity->GetName());
//
//      std::map<std::string, std::set<Entity*>>::iterator i2 = _entityTypesMap.find(entity->GetType());
//      if (i2 != _entityTypesMap.end())
//      {
//        (*i2).second.erase(entity);
//      }    
//      else
//      {
//        throw std::runtime_error((boost::format("type '%s' not found in entity types map") % entity->GetType()).str());
//      }
//
//      _collisionManager->Remove(entity);
//
//      i = _entities.erase(i);
//    }
//    else
//    { 
//      i++;
//    }
//  }
//}

void EntityManager::EnsureEntityNameIsNotRegistered(const std::string& name)
{
  Entity* entity = GetEntityByName(name);
  if (entity != nullptr)
  {
    throw std::runtime_error((boost::format("entity name '%s' is already registered") % name).str());
  }
}
