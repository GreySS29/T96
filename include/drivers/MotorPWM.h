#pragma once

#include "../hardware/IMotor.h"
#include "../hardware/IPwm.h"
#include <gpiod.hpp>
#include <string>
#include <cmath>

using V = gpiod::line::value;
gpiod::line::values vals;

class MotorPWM final : public IMotor
{
public:
    MotorPWM(gpiod::line::offset AIN1,
             gpiod::line::offset AIN2,
             IPwm& pwm,
             const std::string& chip_path);
    ~MotorPWM() override;

    void set_speed(float s) override {
          state_ = MotorState::SpeedMode;
        if (s > 0)      vals = {V::ACTIVE,   V::INACTIVE};
        else if (s < 0) vals = {V::INACTIVE, V::ACTIVE};
        else            vals = {V::INACTIVE, V::INACTIVE};
        request_.set_values(pins_, vals);
        pwm_.set_duty(std::fabs(s));
    }
    void set_state(MotorState new_state) override;
    MotorState get_state() const noexcept override { return state_; };
    static const gpiod::line::offsets& validated(const gpiod::line::offsets& p);

    void brake() override {            
        request_.set_values(pins_, {V::ACTIVE, V::ACTIVE});
        pwm_.set_duty(0.0f);
        state_ = MotorState::Brake;

    }

    void coast() override {            
        request_.set_values(pins_, {V::INACTIVE, V::INACTIVE});
        pwm_.set_duty(1.0f);
        state_ = MotorState::Inactive;
    }


private:
    gpiod::line::offsets pins_;      //vector
    gpiod::chip          chip_;      
    gpiod::line_request  request_;
    IPwm& pwm_;
    MotorState           state_ = MotorState::Inactive;
};