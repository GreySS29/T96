#include "../../include/hardware/Ymotor.h"

Ymotor::~Ymotor()
{
    cleanup();
}


void Ymotor::cleanup() {
        if (request_) {
            gpiod_line_request_release(request_);
            request_ = nullptr;
        }
        if (config_) {
            gpiod_line_config_free(config_);
            config_ = nullptr;
        }
        if (settings_) {
            gpiod_line_settings_free(settings_);
            settings_ = nullptr;
        }
        if (chip_) {
            gpiod_chip_close(chip_);
            chip_ = nullptr;
        }
    }