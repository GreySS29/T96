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
       while (true)
       {
        tank.get_ultra_distant();
        std::this_thread::sleep_for(std::chrono::seconds(1));
        // tank.active_safety();
       }
       tank.stop();


       
       
   } catch (const std::exception& e) {
       std::cerr << "error: " << e.what() << '\n';
       return 1;
   }
}
