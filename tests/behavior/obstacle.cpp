#include "../../include/drivers/HC_SR04.h"
#include "../../include/drivers/MotorDriver_25mm.h"
#include "../../include/hardware/chassis.h"
#include <random>

#include <chrono>
#include <thread>
#include <iostream>

int coin_flip() {
    static std::mt19937 gen{std::random_device{}()};
    static std::uniform_int_distribution<int> dist(1, 2);
    return dist(gen);
}

int main () {
    try {
        Chassis base {
            std::make_unique<MotorDriver_25mm>(17,27),
            std::make_unique<MotorDriver_25mm>(23,24)};
        HC_SR04 ultrasonic;
        
        while(true) {
            auto cm = ultrasonic.distance_cm();
            if (cm && *cm > 20.0) base.forward();
            else 
            {
                int rd = coin_flip();
                if (rd == 1) 
                {
                    base.turn_left_full();
                    std::this_thread::sleep_for(std::chrono::seconds(1));
                    base.stop(); }
                if (rd == 2) 
                {
                    base.turn_right_full();
                    std::this_thread::sleep_for(std::chrono::seconds(1));
                    base.stop();
                }
            }
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
        base.stop();
    }
    catch(const std::exception& e) {
        std::cerr << "error:" << e.what() << '\n';
        return 1;
    }
}
