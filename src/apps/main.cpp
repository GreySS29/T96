#include "../../include/drivers/MotorDriver_25mm.h"

#include <chrono>
#include <thread>

int main()
{
    MotorDriver_25mm motor(17, 18);

    motor.set_state(MotorState::ActiveUp);
    std::this_thread::sleep_for(std::chrono::seconds(1));

    motor.set_state(MotorState::Inactive);

    return 0;
}