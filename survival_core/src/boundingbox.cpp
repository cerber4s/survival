#include "boundingbox.h"

BoundingBox::BoundingBox(double left, double top, double right, double bottom)  : 
  _topLeft(Vector2d(left, top)), 
  _bottomRight(Vector2d(bottom, right)),
  _size(Vector2d(right - left, bottom - top)),
  _center(Vector2d(left + right, top + bottom) * 0.5)
{
}

BoundingBox::BoundingBox(const Vector2d& topLeft, const Vector2d& bottomRight) : 
  _topLeft(topLeft), 
  _bottomRight(bottomRight),
  _size((bottomRight - topLeft)),
  _center(Vector2d(topLeft + bottomRight) * 0.5)
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

double BoundingBox::GetWidth() const
{
  return _size.x;
}

double BoundingBox::GetHeight() const
{
  return _size.y;
}

bool BoundingBox::Contains(double x, double y) const
{
  return x >= GetLeft() && y >= GetTop() &&
    x <= GetRight() && y <= GetBottom();
}

bool BoundingBox::Contains(const Vector2d& position) const
{
  return position.x >= GetLeft() && position.y >= GetTop() &&
    position.x <= GetRight() && position.y <= GetBottom();
}

bool BoundingBox::Contains(const BoundingBox& other) const
{
  return other.GetLeft() >= GetLeft() && other.GetRight() <= GetRight() &&
    other.GetTop() >= GetTop() && other.GetBottom() <= GetBottom();
}
