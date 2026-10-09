#include "../include/Tank.h"

#include <chrono>
#include <thread>
#include <iostream>


//std::this_thread::sleep_for(std::chrono::seconds(1));
int main()
{
    try {
       Tank tank;
       
   } catch (const std::exception& e) {
       std::cerr << "error: " << e.what() << '\n';
       return 1;
   }
}
