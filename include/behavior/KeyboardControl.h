#include "iostream"
#include "../hardware/Chassis.h"
#include "Safety.h" 
#include <atomic>
#include <thread>



class KeyboardControl
{
private:
    char command_ =0;
    Chassis& chassis_;
    Safety& detector_;
public:
    KeyboardControl(Chassis* chassis, Safety* detector) :
        chassis_(chassis), detector_(detector){};
    void take_control(){
        while(command_!='x'){
            if (detector_)
            std::cin >> command_;
            if (command_ == 'w') chassis_->forward();
            else if (command_ == 's') chassis_->stop();
            else if (command_ == 'a') chassis_->turn_left_full();
            else if (command_ == 'd') chassis_->turn_right_full();
        }
        chassis_->stop();
    }
};

