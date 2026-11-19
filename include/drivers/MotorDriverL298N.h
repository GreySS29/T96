#pragma once

#include "../hardware/IMotor.h"
#include <gpiod.hpp>
#include <string>

class MotorDriverL298N final : public IMotor
{
public:
    MotorDriverL298N(gpiod::line::offset pin1,
                     gpiod::line::offset pin2,
                     const std::string& chip_path = "/dev/gpiochip0");
    ~MotorDriverL298N() override;

    void set_speed(float s) = 0;
    void       set_state(MotorState new_state) override;
    MotorState get_state() const noexcept override { return state_; };

private:
    gpiod::line::offsets pins_;      //vector
    gpiod::chip          chip_;      
    gpiod::line_request  request_;
    MotorState           state_ = MotorState::Inactive;
};