#include <memory>
#include "hardware/Chassis.h"
#include "behavior/Safety.h"
#include "behavior/KeyboardControl.h"
#include "drivers/MotorDriver_25mm.h"
#include "drivers/HC_SR04.h"
#include "drivers/Ir_sensor.h"
#include "scenarios/StraightDriving.h"
#include <utility>

class Tank
{
private:
    std::unique_ptr<Chassis> base_;
    std::unique_ptr<ISensor> ultraSonicFront_;
    std::unique_ptr<ISensor> ir_sensorRight_;
    std::unique_ptr<ISensor> ir_sensorLeft_;
    std::unique_ptr<Safety> safety_;
    std::unique_ptr<KeyboardControl> kControl_;
    std::unique_ptr<StraightDriving> stDr_;

public:
    Tank():
    base_(std::make_unique<Chassis>
            (std::make_unique<MotorDriver_25mm>(17,27),
            std::make_unique<MotorDriver_25mm>(23,24))
            ) ,
    ultraSonicFront_ (std::make_unique<HC_SR04>(5,6)),
    ir_sensorRight_(std::make_unique<Ir_sensor>(22)),
    ir_sensorLeft_(std::make_unique<Ir_sensor>(25)),
    safety_(std::make_unique<Safety>(base_,ultraSonicFront_,ir_sensorRight_,ir_sensorLeft_)),
    kControl_(std::make_unique<KeyboardControl>(base_)),
    stDr_(std::make_unique<StraightDriving>(base_,ultraSonicFront_))
    {}

    Tank(std::unique_ptr<Chassis> base, std::unique_ptr<ISensor> front)     //for test
    : base_(std::move(base)), ultraSonicFront_(std::move(front)) {}

    void get_control_on_keyboard() {
        kControl_->take_control();
    }

    void go_straight(std::chrono::seconds second){
        stDr_->active(second);
    }

    void stop(){base_->stop();};
    void get_ultra_distant(){
        std::cout << ultraSonicFront_->read_distance_cm().value_or(-1.0) << '\n';}
   
};

