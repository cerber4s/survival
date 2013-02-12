#include "collisionmanager.h"

#include "entity.h"

CollisionManager::CollisionManager(int maxEntitiesPerCel) :
  _root(new Cell(maxEntitiesPerCel, Vector2d(128 * -1024, 128 * -1024), Vector2d(128 * 1024, 128 * 1024)))
{
}
  
void CollisionManager::Add(Entity* entity)
{
  _root->Add(entity);
}

void CollisionManager::Remove(Entity* entity)
{
  _root->Remove(entity, entity->GetPosition());
}

void CollisionManager::Update(Entity* entity, const Vector2d& previousPosition)
{
  _root->Add(entity);
}

void CollisionManager::Clear()
{
  _root->Clear();
}

void CollisionManager::HandleCollisions()
{
  _root->HandleCollisions();
}
