#include "../include/Tank.h"

#include <chrono>
#include <thread>
#include <iostream>


//std::this_thread::sleep_for(std::chrono::seconds(1));
int main()
{
    
    try {
       Tank tank;
       tank.stop();
       char kcontrol;
       while (std::cin >> kcontrol)
       {
        if (kcontrol == 'k') tank.get_control_on_keyboard();
       }
       tank.stop();
       
       
       
       


       
       
   } catch (const std::exception& e) {
       std::cerr << "error: " << e.what() << '\n';
       return 1;
   }
}
