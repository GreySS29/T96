#include <memory>
#include "hardware/Chassis.h"
#include "hardware/SensorPanel.h"
#include "behavior/Safety.h"
#include "behavior/KeyboardControl.h"
#include "drivers/MotorDriver_25mm.h"


#include "scenarios/StraightDriving.h"
#include <utility>

class Tank
{
private:
    std::unique_ptr<Chassis> base_;
    std::unique_ptr<SensorPanel> sensPanel_;
    std::unique_ptr<Safety> safety_;
    std::unique_ptr<KeyboardControl> kControl_;

public:
    Tank():
    base_(std::make_unique<Chassis>
        (std::make_unique<MotorDriver_25mm>(17,27),
        std::make_unique<MotorDriver_25mm>(23,24))),
    sensPanel_ ( std::make_unique<SensorPanel> 
        (std::make_unique<HC_SR04>(5,6),
        std::make_unique<Ir_sensor>(22),
        std::make_unique<Ir_sensor>(25),
        std::make_unique<Ir_sensor>(00), //add
        std::make_unique<Ir_sensor>(00))), //add
    safety_(std::make_unique<Safety>(base_,sensPanel_)),
    kControl_(std::make_unique<KeyboardControl>(base_)),
    stDr_(std::make_unique<StraightDriving>(base_,sensPanel_))
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

