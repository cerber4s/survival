#include "boundingbox.h"

BoundingBox::BoundingBox(const Vector2d& topLeft, const Vector2d& bottomRight) : 
  _topLeft(topLeft), 
  _bottomRight(bottomRight),
  _size((bottomRight - topLeft)),
  _center((topLeft + bottomRight) * 0.5)
{
}

bool BoundingBox::IsOverlappedWith(const BoundingBox& other)
{
  return !((other.GetTop() > this->GetBottom()) ||
    (other.GetBottom() < this->GetTop()) ||
    (other.GetLeft() > this->GetRight()) ||
    (other.GetRight() < this->GetLeft()));
}

Vector2d BoundingBox::GetTopLeft() const 
{
  return _topLeft;
}

Vector2d BoundingBox::GetBottomRight() const
{
  return _bottomRight;
}

Vector2d BoundingBox::GetSize() const
{
  return _size;
}

Vector2d BoundingBox::GetCenter() const
{
  return _center;
}

double BoundingBox::GetTop() const
{
  return _topLeft.y;
}

double BoundingBox::GetBottom() const
{
  return _bottomRight.y;
}

double BoundingBox::GetLeft() const
{
  return _topLeft.x;
}

double BoundingBox::GetRight() const
{
  return _bottomRight.x;
}
