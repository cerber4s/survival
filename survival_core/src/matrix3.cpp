#include "matrix3.h"

#include <luabind/luabind.hpp>
#include <luabind/operator.hpp>

Matrix3::Matrix3()
{
  for (int i = 0; i < 9; i++) 
  {
    values[i % 3][i - (3 * (i % 3))] = 0;
  }
}

Matrix3::Matrix3(const Matrix3& other)
{
  for (int i = 0; i < 9; i++) 
  {
    int row = i % 3;
    int column = i - (3 * row);
    
    values[row][column] = other.values[row][column];
  }
}

Matrix3::~Matrix3()
{
}

double Matrix3::Get(int row, int column) const
{
  return values[row][column];
}

void Matrix3::Set(int row, int column, double value)
{
  values[row][column] = value;
}

Matrix3& Matrix3::operator=(const Matrix3& other)
{
  for (int i = 0; i < 9; i++) 
  {
    int row = i % 3;
    int column = i - (3 * row);
    
    values[row][column] = other.values[row][column];
  }
  
  return *this;
}

Matrix3 Matrix3::operator*(double factor) const
{
  Matrix3 out;
  int i, j;
  for (i = 0; i < 3; i++) 
  {
    for (j = 0; j < 3; j++)
    {
          out.Set(i, j, out.Get(i, j) * factor);
    }
  }
  return out;
}

Vector2d Matrix3::operator*(const Vector2d& columnVector) const
{
    return Vector2d(
            (Get(0, 0) * columnVector.x) + (Get(0, 1) * columnVector.y) + Get(0, 2),
            (Get(1, 0) * columnVector.x) + (Get(1, 1) * columnVector.y) + Get(1, 2));    
}

Matrix3 Matrix3::operator*(const Matrix3& other) const
{
  Matrix3 out;
  int i, j, k;
  double t;
  for (i = 0; i < 3; i++) 
  {
    for (j = 0; j < 3; j++) 
    {
      t = 0.0;
      for (k = 0; k < 3; k++) 
      {
        t += Get(i, k) * other.Get(k, j);
      }
      out.Set(i, j, t);
    }
  }
  return out;
}

Matrix3 Matrix3::CreateIdentity()
{
  Matrix3 out;
  out.Set(0, 0, 1);
  out.Set(1, 1, 1);
  out.Set(2, 2, 1);

  return out;
}

Matrix3 Matrix3::CreateTranslation(double x, double y)
{
  Matrix3 out = Matrix3::CreateIdentity();
  out.Set(0, 2, x);
  out.Set(1, 2, y);

  return out;
}

Matrix3 Matrix3::CreateTranslation(const Vector2d& translation)
{ 
  return Matrix3::CreateTranslation(translation.x, translation.y);
}

Matrix3 Matrix3::CreateScaling(double x, double y)
{
  Matrix3 out = Matrix3::CreateIdentity();
  out.Set(0, 0, x);
  out.Set(1, 1, y);

  return out;
}

Matrix3 Matrix3::CreateScaling(const Vector2d& scaling)
{
  return Matrix3::CreateScaling(scaling.x, scaling.y);
}

Matrix3 Matrix3::CreateScaling(double scale)
{
  return Matrix3::CreateScaling(scale, scale);
}

Matrix3 Matrix3::CreateRotation(double angleInRadians)
{
  // TODO: implement function to create rotation matrix based on normal vector (for performance reasons (sin/cos calls not needed)
  
  double cos = std::cos(angleInRadians);
  double sin = std::sin(angleInRadians);

  Matrix3 out = Matrix3::CreateIdentity();
  out.Set(0, 0, cos);
  out.Set(0, 1, -sin);
  out.Set(1, 0, sin);
  out.Set(1, 1, cos);

  return out;
}

void Matrix3::RegisterWithLua(lua_State *L)
{
  luabind::module(L)
  [
    luabind::class_<Matrix3>("m3")
      .def(luabind::constructor<>())
      .def(luabind::const_self * double())
      .def(luabind::const_self * luabind::other<Vector2d>())
      .def(luabind::const_self * luabind::const_self)
      .def("get", &Matrix3::Get)
      .def("set", &Matrix3::Set)
      .scope
      [
        luabind::def("create_identity", &Matrix3::CreateIdentity),
        luabind::def("create_translation", (Matrix3(*)(const Vector2d&)) &Matrix3::CreateTranslation),
        luabind::def("create_translation", (Matrix3(*)(double, double)) &Matrix3::CreateTranslation),
        luabind::def("create_scaling", (Matrix3(*)(double, double)) &Matrix3::CreateScaling),
        luabind::def("create_scaling", (Matrix3(*)(const Vector2d&)) &Matrix3::CreateScaling),
        luabind::def("create_scaling", (Matrix3(*)(double)) &Matrix3::CreateScaling),
        luabind::def("create_rotation", &Matrix3::CreateRotation)
      ]
  ];

}