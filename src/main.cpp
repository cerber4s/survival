#include <iostream>
#include <memory>
#include <sstream>

#include <lua.hpp>
#include <luabind/luabind.hpp>

#include "application.h"

int add_file_and_line(lua_State* L);

#define SURVIVAL_OPTION_CATCH_EXCEPTION

int main(int argc, char **argv) 
{
  luabind::set_pcall_callback(add_file_and_line);

#ifdef SURVIVAL_OPTION_CATCH_EXCEPTION  
  try
  {
#endif
    std::unique_ptr<Application> application(new Application());
    if (application->Initialize())
    {
      application->Execute();  
    }
    application->UnInitialize();

#ifdef SURVIVAL_OPTION_CATCH_EXCEPTION
  }
  catch (const luabind::error& e)
  {
    luabind::object error_message(luabind::from_stack(e.state(), -1));
    std::cout << "luabind error: " << e.what() << " - " << error_message << std::endl;
  }
  catch (const std::exception& e)
  {
    std::cout << "error: " << e.what() << std::endl;
  }
  catch (...)
  {
    std::cout << "an unknown error occurred" << std::endl;
  }
#endif

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
