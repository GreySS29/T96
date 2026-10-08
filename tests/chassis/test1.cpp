#include "../../include/hardware/chassis.h" 
#include "../../include/drivers/MotorDriver_25mm.h"

#include <chrono>
#include <thread>
#include <iostream>


int main(){
    try {
    Chassis base {
    std::make_unique<MotorDriver_25mm>(17,27),
    std::make_unique<MotorDriver_25mm>(23,24)
    };
    base.forward();
    std::this_thread::sleep_for(std::chrono::seconds(1));
    base.stop();
    }
    catch(const std::exception& e) {
        std::cerr << "error:" << e.what() << '\n';
        return 1;
    }
}
