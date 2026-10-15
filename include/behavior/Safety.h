#include "../hardware/Chassis.h"
#include "../hardware/ISensor.h"
#include <atomic>
#include <thread>

class Safety
{
private:
    std::unique_ptr<Chassis>& chassis_;
    std::unique_ptr<ISensor>& ultraSonicFront_;
    //std::unique_ptr<ISensor>& ultraSonicBack_;
    std::unique_ptr<ISensor>& ir_sensorRight_;
    std::unique_ptr<ISensor>& ir_sensorLeft_;
    std::atomic<bool> emergency_stop_{false};
    std::jthread watchdog_;
    

public:
    Safety(std::unique_ptr<Chassis>& base,std::unique_ptr<ISensor>& ultraSonicFront,std::unique_ptr<ISensor>& ir_sensorRight,std::unique_ptr<ISensor>& ir_sensorLeft) :
    chassis_(base), ultraSonicFront_(ultraSonicFront),  ir_sensorRight_(ir_sensorRight), ir_sensorLeft_(ir_sensorLeft){}
    ~Safety() {
        stop_watcdog();
    }
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

    void obstacle_watchdog(std::stop_token stop_token){
        while(!stop_token.stop_requested()) {
            if (ultraSonicFront_->check_min_distant_cm(15.0) || ir_sensorLeft_->is_object_detected() 
                || ir_sensorRight_->is_object_detected()){
                    emergency_stop_.store(true);
                    chassis_->stop();
                    break; 
                }
            std::this_thread::sleep_for(std::chrono::milliseconds(50));
        }
    }



};

