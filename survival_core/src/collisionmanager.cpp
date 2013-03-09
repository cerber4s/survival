#include "collisionmanager.h"

#include "entity.h"

CollisionManager::CollisionManager(double left, double top, double right, double bottom, int maxEntitiesPerCel) :
  _root(new Cell(left, top, right, bottom, maxEntitiesPerCel))
{
}

CollisionManager::CollisionManager(const Vector2d& topLeft, const Vector2d& bottomRight, int maxEntitiesPerCel) :
  _root(new Cell(topLeft, bottomRight, maxEntitiesPerCel))
{
}

CollisionManager::CollisionManager(const BoundingBox& bounds, int maxEntitiesPerCel) :
  _root(new Cell(bounds, maxEntitiesPerCel))
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
  _root->Remove(entity, previousPosition);
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

std::set<Entity*> CollisionManager::GetEntitiesInRange(double x, double y, double radius) const
{
  return _root->GetEntitiesInRange(x, y, radius);
}

std::set<Entity*> CollisionManager::GetEntitiesInRange(const Vector2d& position, double radius) const
{
  return _root->GetEntitiesInRange(position, radius);
}
