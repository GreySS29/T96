#include <gpiod.h>
#include <iostream>

int main() {
    gpiod_chip* chip = gpiod_chip_open("/dev/gpiochip0");
    if (!chip) { perror("gpiod_chip_open"); return 1; }

    gpiod_line_settings* settings = gpiod_line_settings_new();
    gpiod_line_config* config = gpiod_line_config_new();
    if (!settings || !config) { perror("new"); return 1; }

    gpiod_line_settings_set_direction(settings, GPIOD_LINE_DIRECTION_OUTPUT);
    gpiod_line_settings_set_output_value(settings, GPIOD_LINE_VALUE_INACTIVE);

    unsigned int pins[] = {17, 27, 23, 24};
    if (gpiod_line_config_add_line_settings(config, pins, 4, settings) < 0) {
        perror("add_line_settings");
        return 1;
    }

    gpiod_request_config* req_cfg = gpiod_request_config_new();
    gpiod_request_config_set_consumer(req_cfg, "motor");

    gpiod_line_request* request = gpiod_chip_request_lines(chip, req_cfg, config);
    if (!request) {
        perror("gpiod_chip_request_lines");  
        return 1;
    }

    auto set = [&](int a, int b, int c, int d) {
        gpiod_line_request_set_value(request, 17, a ? GPIOD_LINE_VALUE_ACTIVE : GPIOD_LINE_VALUE_INACTIVE);
        gpiod_line_request_set_value(request, 27, b ? GPIOD_LINE_VALUE_ACTIVE : GPIOD_LINE_VALUE_INACTIVE);
        gpiod_line_request_set_value(request, 23, c ? GPIOD_LINE_VALUE_ACTIVE : GPIOD_LINE_VALUE_INACTIVE);
        gpiod_line_request_set_value(request, 24, d ? GPIOD_LINE_VALUE_ACTIVE : GPIOD_LINE_VALUE_INACTIVE);
    };

    char in = 0;
    set(0, 0, 0, 0);
    while (in != 'x') {
        std::cin >> in;
        if      (in == 'w') set(1, 0, 1, 0);
        else if (in == 's') set(0, 0, 0, 0);
        else if (in == 'a') set(1, 0, 0, 0);
        else if (in == 'd') set(0, 0, 1, 0);
    }
    set(0, 0, 0, 0);

    gpiod_line_request_release(request);
    gpiod_request_config_free(req_cfg);
    gpiod_line_config_free(config);
    gpiod_line_settings_free(settings);
    gpiod_chip_close(chip);
    return 0;
}
