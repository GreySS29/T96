#pragma once

#include "../hardware/Ymotor.h"


class MotorDriver_25mm final : public Ymotor{
    
    public:
    MotorDriver_25mm(int pin1, int pin2);
    ~MotorDriver_25mm() override =default;

    //set
    void set_state(MotorState new_state) override;

};
