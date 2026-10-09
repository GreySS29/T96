#include <memory>
#include "hardware/Chassis.h"
#include "behavior/Safety.h"
#include "drivers/MotorDriver_25mm.h"
#include "drivers/HC_SR04.h"
#include <utility>

class Tank
{
private:
    std::unique_ptr<Chassis> base_;
    std::unique_ptr<ISensor> ultraSonicFront_;
    std::unique_ptr<Safety> safety_;

public:
    Tank():
    base_(std::make_unique<Chassis>
            (std::make_unique<MotorDriver_25mm>(17,27),
            std::make_unique<MotorDriver_25mm>(23,24))
            ) ,
    ultraSonicFront_ (std::make_unique<HC_SR04>(5,6)),
    safety_(std::make_unique<Safety>(base_,ultraSonicFront_))
    {}

    Tank(std::unique_ptr<Chassis> base, std::unique_ptr<ISensor> front)
    : base_(std::move(base)), ultraSonicFront_(std::move(front)) {}

    void active_safety(){safety_->active();};
    void stop(){base_->stop();};
   
};

