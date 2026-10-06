#include "../include/drivers/MotorDriver_25mm.h"

#include <chrono>
#include <thread>

int main()
{
    try {
       MotorDriver_25mm motor(23, 24);
       motor.set_state(MotorState::ActiveUp);
       std::this_thread::sleep_for(std::chrono::seconds(1));
       motor.set_state(MotorState::Inactive);
   } catch (const std::exception& e) {
       std::cerr << "error: " << e.what() << '\n';
       return 1;
   }
}