#include "../../include/drivers/MotorDriver_25mm.h"
#include <stdexcept>

MotorDriver_25mm::MotorDriver_25mm(gpiod::line::offset pin1,
                                   gpiod::line::offset pin2,
                                   const std::string& chip_path)
    : pins_{pin1, pin2},
      chip_(chip_path),
      request_(chip_.prepare_request()
          .set_consumer("motor-25mm")
          .add_line_settings(pins_,
              gpiod::line_settings()
                  .set_direction(gpiod::line::direction::OUTPUT)
                  .set_output_value(gpiod::line::value::INACTIVE))
          .do_request())
{
    if (pin1 == pin2) {
        throw std::invalid_argument("Motor pins must be different");
    }
    // линии уже в INACTIVE, state_ = Inactive по умолчанию
}

MotorDriver_25mm::~MotorDriver_25mm()
{
    try { set_state(MotorState::Inactive); } catch (...) {}
}

void MotorDriver_25mm::set_state(MotorState new_state)
{
    using V = gpiod::line::value;
    gpiod::line::values vals;

    switch (new_state) {
    case MotorState::ActiveUp:   vals = {V::ACTIVE,   V::INACTIVE}; break;
    case MotorState::Inactive:   vals = {V::INACTIVE, V::INACTIVE}; break;
    case MotorState::ActiveBack: vals = {V::INACTIVE, V::ACTIVE};   break;
    default: throw std::invalid_argument("Invalid motor state");
    }

    request_.set_values(pins_, vals);   
    state_ = new_state;
}