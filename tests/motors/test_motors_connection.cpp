#include <gpiod.h>
#include <unistd.h>

constexpr auto gp_set_value = gpiod_line_request_set_value;

int main ()
{
        gpiod_chip* chip = gpiod_chip_open("/dev/gpiochip0");

        gpiod_line_settings*  settings = gpiod_line_settings_new();
        gpiod_line_config* config = gpiod_line_config_new();
        gpiod_line_settings_set_direction(settings, GPIOD_LINE_DIRECTION_OUTPUT);
        unsigned int pins[] = {17,27,23,24};

        gpiod_line_config_add_line_settings(config, pins,sizeof(pins)/sizeof(pins[0]),settings);
        gpiod_line_request* request = gpiod_chip_request_lines(chip, nullptr, config);

        gp_set_value(request, 17, GPIOD_LINE_VALUE_ACTIVE);
        gp_set_value(request, 27 , GPIOD_LINE_VALUE_INACTIVE);
        sleep(2);

        gp_set_value(request, 17, GPIOD_LINE_VALUE_INACTIVE);
        gp_set_value(request,27, GPIOD_LINE_VALUE_INACTIVE);
        sleep(2);
        gp_set_value(request, 23, GPIOD_LINE_VALUE_ACTIVE);
        gp_set_value(request,24, GPIOD_LINE_VALUE_INACTIVE);

        sleep(2);
        gp_set_value(request,23,GPIOD_LINE_VALUE_INACTIVE);
        gp_set_value(request,24,GPIOD_LINE_VALUE_INACTIVE);


        gpiod_line_request_release(request);
        gpiod_line_config_free(config);
        gpiod_line_settings_free(settings);
        gpiod_chip_close(chip);
        return 0;
}
