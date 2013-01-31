#include <iostream>
#include <memory>

#include "application.h"

//#define SURVIVAL_OPTION_CATCH_EXCEPTION

int main(int argc, char **argv) {

    std::unique_ptr<Application> application(new Application());      
    
#ifdef SURVIVAL_OPTION_CATCH_EXCEPTION
    try
    {
#endif

      if (application->Initialize())
      {
        application->Execute();  
      }
      application->UnInitialize();
      
#ifdef SURVIVAL_OPTION_CATCH_EXCEPTION
    }
    catch (luabind::error e)
    {
      luabind::object error_message(luabind::from_stack(e.state(), -1));
      std::cout << "luabind error: " << e.what() << " - " << error_message << std::endl;
    }
#endif
    
    return 0;
}
