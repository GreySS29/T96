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
        else if (kcontrol =='s') {
            std::cout << "input second >>" << std::endl;
            int second{};
            std::cin >> second; 
            std::chrono::seconds seconds{second};
            
            tank.go_straight(seconds);
        }
       }
       tank.stop();
       
       
       
       


       
       
   } catch (const std::exception& e) {
       std::cerr << "error: " << e.what() << '\n';
       return 1;
   }
}
