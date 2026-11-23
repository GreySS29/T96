#include <memory>
#include <utility>

#include "behavior/Safety.h"
#include "behavior/KeyboardControl.h"
#include "scenarios/StraightDriving.h"
#include "structure/PhysicalBody.h"


class Tank
{
private:
    std::unique_ptr<PhysicalBody> body_; 
    std::unique_ptr<Safety> safety_;
    std::unique_ptr<KeyboardControl> kControl_;
    std::unique_ptr<StraightDriving> stDr_;
    

public:
    Tank():
    body_(std::make_unique<PhysicalBody>()),
    safety_(std::make_unique<Safety>(
        body_->base_,
        body_->ultraSonicFront_,
        body_->ir_sensorRight_,
        body_->ir_sensorLeft_)),
    kControl_(std::make_unique<KeyboardControl>(
        body_->base_)),
    stDr_(std::make_unique<StraightDriving>(
        body_->base_,
        body_->ultraSonicFront_))
    {}

    // Tank(std::unique_ptr<Chassis> base, std::unique_ptr<ISensor> front)     //for test
    // : base_(std::move(base)), ultraSonicFront_(std::move(front)) {}

    void get_control_on_keyboard() {
        kControl_->take_control();
    }

    void go_straight(std::chrono::seconds second){
        stDr_->active(second);
    }

    //temporary function
    void stop(){body_->base_->stop();}; 
    void get_ultra_distant(){
        std::cout << body_->ultraSonicFront_->read_distance_cm().value_or(-1.0) << '\n';}
   
};

