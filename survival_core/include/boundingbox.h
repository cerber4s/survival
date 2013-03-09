#ifndef BOUNDINGBOX_H
#define BOUNDINGBOX_H

#include "vector2d.h"

class BoundingBox
{
public:
  BoundingBox(double left, double top, double right, double bottom);
  BoundingBox(const Vector2d& topLeft, const Vector2d& bottomRight);

  bool IsOverlappedWith(const BoundingBox& other);

  Vector2d GetTopLeft() const;
  Vector2d GetBottomRight() const;
  
  Vector2d GetSize() const;
  
  Vector2d GetCenter() const;

  double GetTop() const;
  double GetBottom() const;
  double GetLeft() const;
  double GetRight() const;

  double GetWidth() const;
  double GetHeight() const;

  bool Contains(double x, double y) const;
  bool Contains(const Vector2d& position) const;
  bool Contains(const BoundingBox& other) const;

private:
  Vector2d _topLeft;
  Vector2d _bottomRight;
  Vector2d _size;
  Vector2d _center;

};

#endif
