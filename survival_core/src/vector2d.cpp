#include "vector2d.h"

#include <luabind/luabind.hpp>
#include <luabind/operator.hpp>

#include "matrix3.h"

#include "common.h"

bool Vector2d::IsZero() const
{
  return ((x*x)+(y*y)) < MinDouble;
}


double Vector2d::Length() const
{
  return std::sqrt((x*x) + (y*y));
}

double Vector2d::LengthSqr() const
{
  return (x*x) + (y*y);
}

Vector2d Vector2d::Normal() const
{
  double vectorLength = Length();
  if (vectorLength > EpsilonDouble)
  {
    return Vector2d(x / vectorLength, y / vectorLength);
  }
  
  return Vector2d(x, y);
}

double Vector2d::Dot(const Vector2d& other) const
{
  return (x*other.x)+(y+other.y);
}

Vector2d Vector2d::Perpendicular() const
{
  return Vector2d(-y, x);
}

Vector2d Vector2d::Truncate(double max) const
{
  if (Length() > max)
  {
    return Normal() * max;
  }
  
  return Vector2d(x, y);
}

double Vector2d::DistanceTo(const Vector2d& other)
{
    double delta_y = other.y - y;
    double delta_x = other.x - x;
    
    return std::sqrt((delta_y*delta_y) + (delta_x*delta_x));
}

double Vector2d::DistanceToSqr(const Vector2d& other)
{
    double delta_y = other.y - y;
    double delta_x = other.x - x;
    
    return (delta_y*delta_y) + (delta_x*delta_x);
}

double Vector2d::Angle()
{
  return std::atan2(y, x);
}

double Vector2d::AngleInDegrees()
{
  double radians = std::atan2(y, x);
  return RadiansToDegrees(radians);
}

Vector2d Vector2d::Rotate(double angleInRadians)
{
    return Matrix3::CreateRotation(angleInRadians) * (*this);
}

Vector2d Vector2d::RotateByDegrees(double angleInDegrees)
{
    return Rotate(DegreesToRadians(angleInDegrees));
}

void Vector2d::RegisterWithLua(lua_State* L)
{
  luabind::module(L)
  [
    luabind::class_<Vector2d>("v2")
      .def(luabind::constructor<>())
      .def(luabind::constructor<double, double>())
      .def(luabind::const_self + luabind::const_self)
      .def(luabind::const_self - luabind::const_self)
      .def(luabind::const_self * double())
      .def(luabind::const_self / double())
      .def("length", &Vector2d::Length)
      .def("length_sqr", &Vector2d::LengthSqr)
      .def("normal", &Vector2d::Normal)
      .def("dot", &Vector2d::Dot)
      .def("perpendicular", &Vector2d::Perpendicular)
      .def("truncate", &Vector2d::Truncate)
      .def("distance_to", &Vector2d::DistanceTo)
      .def("distance_to_sqr", &Vector2d::DistanceToSqr)
      .def("rotate", &Vector2d::Rotate)
      .def("rotate_by_degrees", &Vector2d::RotateByDegrees)
      .def("angle", &Vector2d::Angle)
      .def("angle_in_degrees", &Vector2d::AngleInDegrees)
      .property("x", &Vector2d::x, &Vector2d::x)
      .property("y", &Vector2d::y, &Vector2d::y)
      .property("is_zero", &Vector2d::IsZero)
  ];
}
