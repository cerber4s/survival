#ifndef VECTOR2D_H
#define VECTOR2D_H

#include <lua.hpp>

class Vector2d
{
public:
  double x;
  double y;
  
  Vector2d() : x(0.0), y(0.0) { }
  Vector2d(double x, double y): x(x), y(y) { }
  Vector2d(const Vector2d& other) : x(other.x), y(other.y) { }
  
  bool IsZero() const;
  
  inline double Length() const;
  inline double LengthSqr() const;
  
  inline Vector2d Normal() const;
  
  inline double Dot(const Vector2d& other) const;
  
  inline Vector2d Perpendicular() const;
  inline Vector2d Truncate(double max) const;
  
  double DistanceTo(const Vector2d& other);
  double DistanceToSqr(const Vector2d& other);
  
  double Angle();
  double AngleInDegrees();

  inline Vector2d Rotate(double angleInRadians);
  inline Vector2d RotateByDegrees(double degrees);
  
  const Vector2d& operator+=(const Vector2d& rhs)
  {
    x += rhs.x;
    y += rhs.y;
    
    return *this;
  }
  
  Vector2d operator+(const Vector2d& rhs)
  {
    return Vector2d(x + rhs.x, y + rhs.y);
  }

  const Vector2d& operator-=(const Vector2d &rhs)
  {
    x -= rhs.x;
    y -= rhs.y;
    
    return *this;
  }
  
  Vector2d operator-(const Vector2d& rhs)
  {
    return Vector2d(x - rhs.x, y - rhs.y);
  }

  Vector2d operator*(double rhs)
  {
    return Vector2d(x * rhs, y * rhs);
  }

  static void RegisterWithLua(lua_State* L);
  
};

inline Vector2d operator+(const Vector2d &lhs, const Vector2d &rhs)
{
  return Vector2d(lhs.x + rhs.x, lhs.y + rhs.y);
}

inline Vector2d operator-(const Vector2d &lhs, const Vector2d &rhs)
{
  return Vector2d(lhs.x - rhs.x, lhs.y - rhs.y);
}

inline Vector2d operator*(const Vector2d &lhs, double rhs)
{
  return Vector2d(lhs.x * rhs, lhs.y * rhs);
}

#endif //VECTOR2D_H