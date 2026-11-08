#include "../hardware/Chassis.h"
#include "../hardware/SensorPanel.h"
#include <atomic>
#include <thread>
#include <chrono>
#include <iostream>

class Safety
{
private:
    std::unique_ptr<Chassis>& chassis_;
    std::unique_ptr<SensorPanel>& sensPanel_;
    std::jthread watchdog_;

   void obstacle_watchdog(std::stop_token stop_token) {
    while (!stop_token.stop_requested()) {
        const bool front = sensPanel_ ->ultraSonicFront_->check_min_distant_cm(15.0);
        const bool left  = sensPanel_->ir_sensorLeft_->is_object_detected();
        const bool right = sensPanel_->ir_sensorRight_->is_object_detected();
        const bool rightBack = sensPanel_->ir_sensorRightBack_->is_object_detected();
        const bool leftBack = sensPanel_->ir_sensorLeftBack_->is_object_detected();

        if (front || left || right || rightBack || leftBack) {
            chassis_->set_emergency_stop(true);
            // std::cout << "obstacle\n";
            save(front, left, right, rightBack, leftBack);
            chassis_->set_emergency_stop(false);  
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
    }
}

    

public:
    Safety(std::unique_ptr<Chassis>& base,
        std::unique_ptr<SensorPanel>& sensPanel,) :
        chassis_(base), 
        sensPanel_(sensPanel), 
        watchdog_([this](std::stop_token stop_token) {
        obstacle_watchdog(stop_token);}) {}
   
    Safety(const Safety&) = delete;
    Safety operator=(const Safety&) = delete;

    
    void save(bool front, bool left, bool right, bool rightBack, bool leftBack) {
    if (front) chassis_->back(1);
    if (left)  chassis_->turn_right_full(1);
    if (right) chassis_->turn_left_full(1);
    if (rightBack) chassis_->turn_left_full(1);
    if (leftBack) chassis_ -> turn_right_full (1);
    }
};

