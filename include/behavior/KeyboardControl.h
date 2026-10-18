#include "iostream"
#include "../hardware/Chassis.h"
#include "memory"



class KeyboardControl
{
private:
    char command_ =0;
    std::unique_ptr<Chassis>& chassis_;
public:
    KeyboardControl(std::unique_ptr<Chassis>& chassis) :
        chassis_(chassis){};
    void take_control(){

        while(command_!='x'){
            std::cin >> command_;

            if (command_ == 'w') chassis_->forward();
            else if (command_ == 's') chassis_->stop();
            else if (command_ == 'a') chassis_->turn_left_full();
            else if (command_ == 'd') chassis_->turn_right_full();
        }
        chassis_->stop();
    }
};

