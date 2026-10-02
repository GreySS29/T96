#include "../../include/drivers/Motor_driver.h"

Motor_driver Motor_driver::Motor_driver(int pin1 , int pin2) : state(Motor_state::Inactive){
    pins[0] = pin1;
    pins[1] = pin2;

    chip = gpiod_chip_open("/dev/gpiochip0");
    if (!chip) {
        throw std::runtime_error("Failed to open gpiochip0");
    }

    settings = gpiod_line_settings_new();
    if (!settings) {
        gpiod_chip_close(chip);
        throw std::runtime_error("Failed to create line settings");
    }

    int ret = gpiod_line_settings_set_direction(settings, GPIOD_LINE_DIRECTION_OUTPUT);
    if (ret < 0) {
        cleanup();
        throw std::runtime_error("Failed to set line direction");
    }

    config = gpiod_line_config_new();
    if (!config) {
        cleanup();
        throw std::runtime_error("Failed to create line config");
    }

    ret = gpiod_line_config_add_line_settings(config, pins, 2, settings);
    if (ret < 0) {
        cleanup();
        throw std::runtime_error("Failed to add line settings");
    }

    request = gpiod_chip_request_lines(chip, nullptr, config);
    if (!request) {
        cleanup();
        throw std::runtime_error("Failed to request lines");
    }

    //initial state
    gpiod_line_request_set_value(request, 0, GPIOD_LINE_VALUE_INACTIVE);
    gpiod_line_request_set_value(request, 1, GPIOD_LINE_VALUE_INACTIVE);
}

   void Motor_driver::set_state(Motor_state new_state) {
    state = new_state;
    switch (state) {
        case Motor_state::ActiveUp:
            gpiod_line_request_set_value(request, 0, GPIOD_LINE_VALUE_ACTIVE);
            gpiod_line_request_set_value(request, 1, GPIOD_LINE_VALUE_INACTIVE);
            break;
        case Motor_state::Inactive:
            gpiod_line_request_set_value(request, 0, GPIOD_LINE_VALUE_INACTIVE);
            gpiod_line_request_set_value(request, 1, GPIOD_LINE_VALUE_INACTIVE);
            break;
        case Motor_state::ActiveBack:
            gpiod_line_request_set_value(request, 0, GPIOD_LINE_VALUE_INACTIVE);
            gpiod_line_request_set_value(request, 1, GPIOD_LINE_VALUE_ACTIVE);
            break;
        }
    }

void Motor_driver::cleanup() {
        if (request) {
            gpiod_line_request_release(request);
            request = nullptr;
        }
        if (config) {
            gpiod_line_config_free(config);
            config = nullptr;
        }
        if (settings) {
            gpiod_line_settings_free(settings);
            settings = nullptr;
        }
        if (chip) {
            gpiod_chip_close(chip);
            chip = nullptr;
        }
    }