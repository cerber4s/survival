#ifndef CELL_H
#define CELL_H

#include "boundingbox.h"
#include "vector2d.h"

#include <set>

class Entity;

class Cell
{
public:
  Cell(int left, int top, int right, int bottom, int maxEntities);
  Cell(const Vector2d& topLeft, const Vector2d& bottomRight, int maxEntities);
  Cell(const BoundingBox& bounds, int maxEntities);
  virtual ~Cell();

  void Add(Entity* entity);
  void Remove(Entity* entity, const Vector2d& previousPosition);
  void Update(Entity* entity, const Vector2d& previousPosition);

  void Clear();

  const BoundingBox& GetBounds() const;

  const std::set<Entity*>& GetEntities() const;
  std::set<Entity*> GetEntitiesInRange(double x, double y, double radiusSqr) const;
  std::set<Entity*> GetEntitiesInRange(const Vector2d& position, double radiusSqr) const;

  void HandleCollisions();

private:
  BoundingBox _bounds;
  int _maxEntities;
  std::set<Entity*> _entities;
  std::set<Entity*> _oversizedEntities;
  Cell** _subCells;

  void ClearSubCells();
  int GetCellIndex(const Vector2d& position);

  void AddToSubCell(Entity* entity);

  void HandleCollisions(const std::set<Entity*>& oversizedEntities);
  void GetEntitiesInRange(const Vector2d& position, double radius, std::set<Entity*>& entitiesInRange) const;

  void ValidateBounds(const Vector2d& position);

};

#endif
