#include <iostream>
#include <memory>

#include "application.h"

int main(int argc, char **argv) {

    std::unique_ptr<Application> application(new Application());      
    try
    {
      if (application->Initialize())
      {
        application->Execute();  
      }
      application->UnInitialize();
    }
    catch (luabind::error e)
    {
      luabind::object error_message(luabind::from_stack(e.state(), -1));
      std::cout << "luabind error: " << e.what() << " - " << error_message << std::endl;
    }
    
    return 0;
}
