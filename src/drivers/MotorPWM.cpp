#include "../../include/drivers/MotorPWM.h"

#include <algorithm>
#include <cmath>
#include <stdexcept>

MotorPWM::MotorPWM(gpiod::line::offset AIN1,
                   gpiod::line::offset AIN2,
                   IPwm& pwm,
                   const std::string& chip_path)
    : pins_(validated({AIN1, AIN2})),
      pwm_(pwm),
      chip_(chip_path),
      request_(chip_.prepare_request()
          .set_consumer("MotorPWM")
          .add_line_settings(pins_,
              gpiod::line_settings()
                  .set_direction(gpiod::line::direction::OUTPUT)
                  .set_output_value(gpiod::line::value::INACTIVE))
          .do_request())
{}

MotorPWM::~MotorPWM()
{
    try { set_state(MotorState::Inactive); } catch (...) {}
}

gpiod::line::offsets MotorPWM::validated(const gpiod::line::offsets& p)
{
    auto s = p;
    std::sort(s.begin(), s.end());
    if (std::adjacent_find(s.begin(), s.end()) != s.end())
        throw std::invalid_argument("Motor pins must be different");
    return p;
}

void MotorPWM::write_pins(V in1, V in2)
{
    request_.set_values(pins_, gpiod::line::values{in1, in2});
}

void MotorPWM::set_speed(float s)
{
    if (s > 0)      write_pins(V::ACTIVE,   V::INACTIVE);
    else if (s < 0) write_pins(V::INACTIVE, V::ACTIVE);
    else            write_pins(V::INACTIVE, V::INACTIVE);

    pwm_.set_duty(std::fabs(s));
    state_ = MotorState::SpeedMode;
}

void MotorPWM::set_state(MotorState new_state)
{
    switch (new_state) {
        //didn't check how work without PWM 
    case MotorState::ActiveUp:   write_pins(V::ACTIVE,   V::INACTIVE); break; 
    case MotorState::Inactive:   write_pins(V::INACTIVE, V::INACTIVE); break;
    case MotorState::ActiveBack: write_pins(V::INACTIVE, V::ACTIVE);   break;
    default: throw std::invalid_argument("Invalid motor state");
    }
    pwm_.set_duty(1.0f);
    state_ = new_state;
}

void MotorPWM::brake()
{
    write_pins(V::ACTIVE, V::ACTIVE);
    pwm_.set_duty(0.0f);
    state_ = MotorState::Brake;
}

void MotorPWM::coast()
{
    write_pins(V::INACTIVE, V::INACTIVE);
    pwm_.set_duty(1.0f);
    state_ = MotorState::Inactive;
}