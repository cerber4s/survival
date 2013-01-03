#ifndef COLOR_H
#define COLOR_H

#include <lua.hpp>

class Color
{
public:
  unsigned char r;
  unsigned char g;
  unsigned char b;
  float a;
  
  Color() : r(0), g(0), b(0), a(255) {}
  Color(unsigned char r, unsigned char g, unsigned char b) : r(r), g(g), b(b), a(1.0) {}
  Color(unsigned char r, unsigned char g, unsigned char b, float a) : r(r), g(g), b(b), a(a) {}
  
  static void RegisterWithLua(lua_State* L);
  
};

#endif // COLOR_H