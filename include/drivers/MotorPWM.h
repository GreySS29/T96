#pragma once

#include "../hardware/IMotorPWM.h"
#include "../hardware/IPwm.h"
#include <gpiod.hpp>
#include <string>

class MotorPWM final : public IMotorPWM
{
public:
    MotorPWM(gpiod::line::offset AIN1,
             gpiod::line::offset AIN2,
             IPwm& pwm,
             const std::string& chip_path);
    ~MotorPWM() override;

    // Owns a GPIO line request, so copying makes no sense.
    MotorPWM(const MotorPWM&) = delete;
    MotorPWM& operator=(const MotorPWM&) = delete;

    void set_speed(float s) override;
    void set_state(MotorState new_state) override;
    MotorState get_state() const noexcept override { return state_; }
    void brake() override;
    void coast() override;

private:
    using V = gpiod::line::value;

    static gpiod::line::offsets validated(const gpiod::line::offsets& p);
    void write_pins(V in1, V in2);

    gpiod::line::offsets pins_;
    IPwm&                pwm_;
    gpiod::chip          chip_;
    gpiod::line_request  request_;
    MotorState           state_ = MotorState::Inactive;
};