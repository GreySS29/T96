#pragma once

#include "../hardware/IMotor.h"
#include <gpiod.hpp>
#include <string>

class MotorDriver_25mm final : public IMotor
{
public:
    MotorDriver_25mm(gpiod::line::offset pin1,
                     gpiod::line::offset pin2,
                     const std::string& chip_path = "/dev/gpiochip0");
    ~MotorDriver_25mm() override;

    void       set_state(MotorState new_state) override;
    MotorState get_state() const noexcept override { return state_; };

private:
    gpiod::line::offsets pins_;      //vector
    gpiod::chip          chip_;      
    gpiod::line_request  request_;
    MotorState           state_ = MotorState::Inactive;
};