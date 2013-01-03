#include "color.h"

#include <luabind/luabind.hpp>

void Color::RegisterWithLua(lua_State* L)
{
  luabind::module(L)
  [
    luabind::class_<Color>("color")
      .def(luabind::constructor<>())
      .def(luabind::constructor<unsigned char, unsigned char, unsigned char>())
      .def(luabind::constructor<unsigned char, unsigned char, unsigned char, float>())
      .property("r", &Color::r, &Color::r)
      .property("g", &Color::g, &Color::g)
      .property("b", &Color::b, &Color::b)
      .property("a", &Color::a, &Color::a)
  ];  
}