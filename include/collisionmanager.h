#ifndef COLLISIONMANAGER_H
#define COLLISIONMANAGER_H

#include <algorithm>
#include <list>
#include <memory>
#include <set>
#include <vector>

#include <boost/foreach.hpp>

#include "boundingbox.h"
#include "common.h"
#include "vector2d.h"

template <class entity_type>
class Cell
{
public:
  Cell(int maxEntities, const Vector2d& topLeft, const Vector2d& bottomRight);
  virtual ~Cell();

  void Add(const entity_type& entity);
  void Remove(const entity_type& entity, const Vector2d& previousPosition);
  void Update(const entity_type& entity, const Vector2d& previousPosition);

  void Clear();

  const std::set<entity_type>& GetEntities() const;

  void HandleCollisions();

//private:
  BoundingBox _boundingBox;
  int _maxEntities;
  std::set<entity_type> _entities;
  Cell** _subCells;

  void ClearSubCells();
  int GetCellIndex(const Vector2d& position);

};

template <class entity_type>
Cell<entity_type>::Cell(int maxEntities, const Vector2d& topLeft, const Vector2d& bottomRight) :
  _maxEntities(maxEntities),
  _boundingBox(topLeft, bottomRight),
  _subCells(nullptr)
{
}

template <class entity_type>
Cell<entity_type>::~Cell()
{
  _entities.clear();
  ClearSubCells();
}

template <class entity_type>
void Cell<entity_type>::Add(const entity_type& entity)
{
  if (_subCells != nullptr)
  {
    _subCells[GetCellIndex(entity->GetPosition())]->Add(entity);
    return;
  }

  Vector2d size = _boundingBox.GetSize();
  if (_entities.size() < _maxEntities || size.x < 32 || size.y < 32)
  {
    _entities.insert(entity);
  }
  else
  {
    if (_subCells == nullptr)
    {
      /*
      Vector2d offsets[] = {
        Vector2d(0.0, 0.0),
        Vector2d(size.x * 0.5, 0.0),
        Vector2d(0.0, size.y * 0.5),
        Vector2d(size.x * 0.5, size.y * 0.5)
      };
      
      Vector2d _topLeft = _boundingBox.GetTopLeft();
      
      _subCells = new Cell*[4];
      for(int i = 0; i < 4; i++)
      {
        _subCells[i] = new Cell(_maxEntities, _topLeft + offsets[i], _topLeft + offsets[i] + (size * 0.5));
      }
      */
      auto topleft = _boundingBox.GetTopLeft();
      auto size = _boundingBox.GetSize();
      auto halfsize = size * 0.5;
      auto center = _boundingBox.GetCenter();

      _subCells = new Cell*[4];
      _subCells[0] = new Cell(_maxEntities, topleft, topleft + halfsize);
      _subCells[1] = new Cell(_maxEntities, topleft + Vector2d(halfsize.x, 0), topleft + Vector2d(halfsize.x, 0) + halfsize);
      
      _subCells[2] = new Cell(_maxEntities, topleft + Vector2d(0, halfsize.y), topleft + Vector2d(0, halfsize.y) + halfsize);
      _subCells[3] = new Cell(_maxEntities, topleft + halfsize, topleft + size);

    }
    
    BOOST_FOREACH(entity_type e, _entities)
    {
      _subCells[GetCellIndex(e->GetPosition())]->Add(e);
    }
    _entities.clear();

    _subCells[GetCellIndex(entity->GetPosition())]->Add(entity);
  }
}

template <class entity_type>
void Cell<entity_type>::Remove(const entity_type& entity, const Vector2d& previousPosition)
{
  if (_subCells == nullptr)
  {
    _entities.erase(entity);
  }
  else
  {
    _subCells[GetCellIndex(previousPosition)]->Remove(entity, previousPosition);
  }
}

template <class entity_type>
void Cell<entity_type>::Update(const entity_type& entity, const Vector2d& previousPosition)
{
  if (_subCells == nullptr)
  {
    _entities.erase(entity);
  }
  else
  {
    _subCells[GetCellIndex(previousPosition)]->Remove(entity, previousPosition);
  }

  Add(entity);
}

template <class entity_type>
void Cell<entity_type>::Clear()
{
  ClearSubCells();
  _entities.clear();
}

template <class entity_type>
const std::set<entity_type>& Cell<entity_type>::GetEntities() const
{
  return _entities;
}

template <class entity_type>
void Cell<entity_type>::HandleCollisions()
{
  if (_subCells != nullptr)
  {
    for(int i = 0; i < 4; i++)
    {
      _subCells[i]->HandleCollisions();
    }

    return;
  }

  std::set<entity_type>::iterator i = _entities.begin();
  while (i != _entities.end())
  {
    std::set<entity_type>::iterator j = std::set<entity_type>::iterator(i);
    j++;

    while (j != _entities.end())
    {
      entity_type left = (*i);
      entity_type right = (*j);
          
      if (left->IsCollidable() && right->IsCollidable() && left->HasCollidedWith(right))
      {
        left->HandleCollisionWith(right);
      }

      j++;
    }
    i++;
  }
}

template <class entity_type>
void Cell<entity_type>::ClearSubCells()
{
  if (_subCells != nullptr)
  {
    for(int i = 0; i < 4; i++)
    {
      delete _subCells[i];
    }

    delete [] _subCells;
    _subCells = nullptr;
  }
}

template <class entity_type>
int Cell<entity_type>::GetCellIndex(const Vector2d& position)
{
  Vector2d center = _boundingBox.GetCenter();

  if (position.x <= center.x)
  {
    if (position.y <= center.y)
      return 0;

    return 2;
  }

  if (position.y <= center.y)
    return 1;

  return 3;
}

template <class entity_type>
class CollisionManager
{
public:
  CollisionManager(int maxEntitiesPerCel);
  
  void Add(const entity_type& entity);
  void Remove(const entity_type& entity);
  void Update(const entity_type& entity, const Vector2d& previousPosition);

  void Clear();

  void HandleCollisions();

//private:
  std::unique_ptr<Cell<entity_type>> _root;

};

template <class entity_type>
CollisionManager<entity_type>::CollisionManager(int maxEntitiesPerCel) :
  _root(new Cell<entity_type>(maxEntitiesPerCel, Vector2d(64 * -1024, 64 * -1024), Vector2d(64 * 1024, 64 * 1024)))
{
}
  
template <class entity_type>
void CollisionManager<entity_type>::Add(const entity_type& entity)
{
  _root->Add(entity);
}

template <class entity_type>
void CollisionManager<entity_type>::Remove(const entity_type& entity)
{
  _root->Remove(entity, entity->GetPosition());
}

template <class entity_type>
void CollisionManager<entity_type>::Update(const entity_type& entity, const Vector2d& previousPosition)
{
  _root->Add(entity);
}

template <class entity_type>
void CollisionManager<entity_type>::Clear()
{
  _root->Clear();
}

template <class entity_type>
void CollisionManager<entity_type>::HandleCollisions()
{
  _root->HandleCollisions();
}

#endif
