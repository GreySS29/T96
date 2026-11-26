//All connections of physical components
#include <memory>
#include <utility>
#include <string>

#include "../hardware/Chassis.h"
#include "../hardware/SoftPwm.h"
#include "../drivers/MotorPWM.h"
#include "../drivers/HC_SR04.h"
#include "../drivers/Ir_sensor.h"
#include "../drivers/StandbyPin.h"




class PhysicalBody
{
private:
    const std::string chip_path_ = "/dev/gpiochip0"; //rasp pi 4
    StandbyPin stby_{26, chip_path_}; 
    SoftPwm pwm_r = SoftPwm(22,chip_path_);
    SoftPwm pwm_l = SoftPwm(25,chip_path_);
    
public:
    std::unique_ptr<Chassis> base_;
    std::unique_ptr<ISensor> ultraSonicFront_;
    std::unique_ptr<ISensor> ir_sensorRight_;
    std::unique_ptr<ISensor> ir_sensorLeft_;

    PhysicalBody() : 
    base_(std::make_unique<Chassis>(
            std::make_unique<MotorPWM>(17,27,pwm_r,chip_path_),
            std::make_unique<MotorPWM>(23,24,pwm_l,chip_path_))),
    ultraSonicFront_ (std::make_unique<HC_SR04>(5,6,chip_path_)), //trig, echo
    ir_sensorRight_(std::make_unique<Ir_sensor>(8,chip_path_)),
    ir_sensorLeft_(std::make_unique<Ir_sensor>(7,chip_path_)){}
};

