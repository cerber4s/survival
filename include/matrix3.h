#ifndef MATRIX3_H
#define MATRIX3_H

#include <lua.hpp>

#include "vector2d.h"

class Matrix3
{
public:
  double values [3][3];
  
  Matrix3();
  Matrix3(const Matrix3& other);
  virtual ~Matrix3();
  
  double Get(int row, int column) const;
  void Set(int row, int column, double value);
  
  Matrix3& operator=(const Matrix3& other);
  Matrix3 operator*(double factor) const;
  Vector2d operator*(const Vector2d& columnVector) const;
  Matrix3 operator*(const Matrix3& other) const;
  
  static Matrix3 CreateIdentity();
  static Matrix3 CreateTranslation(double x, double y);
  static Matrix3 CreateTranslation(const Vector2d& translation);
  static Matrix3 CreateScaling(double x, double y);
  static Matrix3 CreateScaling(const Vector2d& scaling);
  static Matrix3 CreateScaling(double scale);
  static Matrix3 CreateRotation(double angleInRadians);
  
  static void RegisterWithLua(lua_State *L);
  
private:
  
};

#endif