#include "../hardware/Chassis.h"
#include "../hardware/ISensor.h"
#include <atomic>
#include <thread>
#include <chrono>

class Safety
{
private:
    std::unique_ptr<Chassis>& chassis_;
    std::unique_ptr<ISensor>& ultraSonicFront_;
    //std::unique_ptr<ISensor>& ultraSonicBack_;
    std::unique_ptr<ISensor>& ir_sensorRight_;
    std::unique_ptr<ISensor>& ir_sensorLeft_;
    std::jthread watchdog_;

    void obstacle_watchdog(std::stop_token stop_token){
    while(!stop_token.stop_requested()) {
        const bool obstacle =
                ultraSonicFront_->check_min_distant_cm(15.0) ||
                ir_sensorLeft_->is_object_detected() ||
                ir_sensorRight_->is_object_detected();

            if(obstacle) {
                chassis_->set_emergency_stop(true);
                break;
            }
        
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
    }
}
    

public:
    Safety(std::unique_ptr<Chassis>& base,
        std::unique_ptr<ISensor>& ultraSonicFront,
        std::unique_ptr<ISensor>& ir_sensorRight,
        std::unique_ptr<ISensor>& ir_sensorLeft) :
        chassis_(base), 
        ultraSonicFront_(ultraSonicFront),  
        ir_sensorRight_(ir_sensorRight), 
        ir_sensorLeft_(ir_sensorLeft), 
        watchdog_([this](std::stop_token stop_token) {
        obstacle_watchdog(stop_token);}) {}
   
    Safety(const Safety&) = delete;
    Safety operator=(const Safety&) = delete;

    
    void save_waiting(){
        // if(ultraSonicFront_->check_min_distant_cm(20.)&& ultraSonicBack_->check_min_distant_cm(20.))
        // {
        //     chassis_->turn_left_full(1);
        //     chassis_->forward(2);
        //     chassis_->turn_right_full(1);
        // }
        if(ultraSonicFront_->check_min_distant_cm(20.0)) chassis_->back(1);
        if(ir_sensorLeft_->is_object_detected()) chassis_-> turn_left_full(1);
        if(ir_sensorRight_->is_object_detected()) chassis_->turn_right_full(1);
        // if(ultraSonicBack_->check_min_distant_cm(20.)) chassis_->forward(1);
    }

   

    



};

