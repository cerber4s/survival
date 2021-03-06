#if WIN32
#define _CRTDBG_MAP_ALLOC 
#define _CRTDBG_MAPALLOC
#include <stdlib.h>
#include <crtdbg.h>
#endif

#include <iostream>
#include <memory>
#include <sstream>

#include <lua.hpp>
#include <luabind/luabind.hpp>

#include <allegro5/allegro.h>

#include "application.h"

int add_file_and_line(lua_State* L);

int main(int argc, char **argv) 
{
#if WIN32
  _CrtSetReportMode( _CRT_ERROR, _CRTDBG_MODE_DEBUG );
  _CrtSetDbgFlag ( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );
#endif

  luabind::set_pcall_callback(add_file_and_line);  
  
  try
  {
    std::unique_ptr<Application> application(new Application());
    if (application->Initialize())
    {
      application->Execute();
      application->UnInitialize();
    }
  }
  catch (const luabind::error& e)
  {
    luabind::object error_message(luabind::from_stack(e.state(), -1));
    std::cerr << "luabind error: " << e.what() << " - " << error_message << std::endl;
  }
  catch (const std::exception& e)
  {
    std::cerr << "error: " << e.what() << std::endl;
  }
  catch (...)
  {
    std::cerr << "an unknown error occurred" << std::endl;
  }

  return 0;
}

int add_file_and_line(lua_State* L)
{
  lua_Debug d;
  lua_getstack(L, 1, &d);
  lua_getinfo(L, "Sln", &d);
  std::string err = lua_tostring(L, -1);
  lua_pop(L, 1);
  std::stringstream msg;
  msg << d.short_src << ":" << d.currentline;

  if (d.name != 0)
  {
    msg << "(" << d.namewhat << " " << d.name << ")";
  }
  msg << " " << err;

  std::cout << msg.str() << std::endl;
  lua_pushstring(L, msg.str().c_str());

  return 1;
}
