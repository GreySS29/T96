#include "../../include/drivers/MotorPWM.h"
#include <stdexcept>

MotorPWM::MotorPWM  (gpiod::line::offset AIN1,
                    gpiod::line::offset AIN2,
                    IPwm& pwm,
                    const std::string& chip_path)
    : pins_{validated({AIN1, AIN2})},
      chip_(chip_path),
      request_(chip_.prepare_request()
          .set_consumer("MotorPWM")
          .add_line_settings(pins_,
              gpiod::line_settings()
                  .set_direction(gpiod::line::direction::OUTPUT)
                  .set_output_value(gpiod::line::value::INACTIVE))
          .do_request())
{
    
    //must be a check of correct connection
    // if () {
    //     throw std::invalid_argument("Motor pins must be different");
    // }
    
}

MotorPWM::~MotorPWM()
{
    try { set_state(MotorState::Inactive); } catch (...) {}
}

void MotorPWM::set_state(MotorState new_state)
{
    using V = gpiod::line::value;
    gpiod::line::values vals;

    switch (new_state) {
    case MotorState::ActiveUp:   vals = {V::ACTIVE,   V::INACTIVE, V::ACTIVE}; break;
    case MotorState::Inactive:   vals = {V::INACTIVE, V::INACTIVE, V::ACTIVE}; break;
    case MotorState::ActiveBack: vals = {V::INACTIVE, V::ACTIVE,  V::ACTIVE}; break;
    default: throw std::invalid_argument("Invalid motor state");
    }

    request_.set_values(pins_, vals);   
    state_ = new_state;
}

void set_speed(float s) override 
{
        state_ = MotorState::SpeedMode;
        using V = gpiod::line::value;
        gpiod::line::values vals;
        if (s > 0)      vals = {V::ACTIVE,   V::INACTIVE};
        else if (s < 0) vals = {V::INACTIVE, V::ACTIVE};
        else            vals = {V::INACTIVE, V::INACTIVE};
        request_.set_values(pins_, vals);
        pwm_.set_duty(std::fabs(s));
}

static const gpiod::line::offsets& validated(const gpiod::line::offsets& p) {
    auto s = p;
    std::sort(s.begin(), s.end());
    if (std::adjacent_find(s.begin(), s.end()) != s.end())
        throw std::invalid_argument("Motor pins must be different");
    return p;
}