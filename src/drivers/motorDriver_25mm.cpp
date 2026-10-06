#include "../../include/drivers/MotorDriver_25mm.h"

MotorDriver_25mm::MotorDriver_25mm(int pin1 , int pin2){
    if (pin1 <0 || pin2 <0) {
        throw std::invalid_argument("GPIO pin numbers cannot be negative");
    }

    pins_[0] = pin1;
    pins_[1] = pin2;

    chip_ = gpiod_chip_open("/dev/gpiochip0");
    if (!chip_) {
        throw std::runtime_error("Failed to open gpiochip0");
    }

    settings_ = gpiod_line_settings_new();
    if (!settings_) {
        gpiod_chip_close(chip_);
        throw std::runtime_error("Failed to create line settings");
    }

    int ret = gpiod_line_settings_set_direction(settings_, GPIOD_LINE_DIRECTION_OUTPUT);
    if (ret < 0) {
        cleanup();
        throw std::runtime_error("Failed to set line direction");
    }

    config_ = gpiod_line_config_new();
    if (!config_) {
        cleanup();
        throw std::runtime_error("Failed to create line config");
    }

    ret = gpiod_line_config_add_line_settings(config_, pins_, 2, settings_);
    if (ret < 0) {
        cleanup();
        throw std::runtime_error("Failed to add line settings");
    }

    request_ = gpiod_chip_request_lines(chip_, nullptr, config_);
    if (!request_) {
        cleanup();
        throw std::runtime_error("Failed to request lines");
    }

    set_state(MotorState::Inactive);
}

    void MotorDriver_25mm::set_state(MotorState new_state)
{
    enum gpiod_line_value value0;
    enum gpiod_line_value value1;

    switch (new_state) {
    case MotorState::ActiveUp:
        value0 = GPIOD_LINE_VALUE_ACTIVE;
        value1 = GPIOD_LINE_VALUE_INACTIVE;
        break;

    case MotorState::Inactive:
        value0 = GPIOD_LINE_VALUE_INACTIVE;
        value1 = GPIOD_LINE_VALUE_INACTIVE;
        break;

    case MotorState::ActiveBack:
        value0 = GPIOD_LINE_VALUE_INACTIVE;
        value1 = GPIOD_LINE_VALUE_ACTIVE;
        break;

    default:
        throw std::invalid_argument("Invalid motor state");
    }

    enum gpiod_line_value values[2] = {value0, value1};

    if (gpiod_line_request_set_values(request_, values) < 0) {
        throw std::runtime_error("Failed to set motor pins");
    }
    state_ = new_state;
}


