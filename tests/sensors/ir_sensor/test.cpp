#include <gpiod.hpp>

#include <chrono>
#include <csignal>
#include <iostream>
#include <thread>

constexpr const char* CHIP_PATH = "/dev/gpiochip0";
constexpr unsigned int IR_GPIO = 22;  

volatile std::sig_atomic_t running = true;

void stopProgram(int)
{
    running = false;
}

int main()
{
    std::signal(SIGINT, stopProgram);

    try {
        gpiod::chip chip(CHIP_PATH);

        gpiod::line_settings settings;
        settings.set_direction(gpiod::line::direction::INPUT);

        gpiod::line_config lineConfig;
        lineConfig.add_line_settings(IR_GPIO, settings);

        gpiod::request_config requestConfig;
        requestConfig.set_consumer("ir-sensor-test");

        auto request = chip.prepare_request()
            .set_request_config(requestConfig)
            .set_line_config(lineConfig)
            .do_request();

        std::cout << "IR GPIO" << IR_GPIO << '\n';


        gpiod::line::value previousValue =
            request.get_value(IR_GPIO);

        while (running) {
            const auto value = request.get_value(IR_GPIO);

            if (value != previousValue) {
                if (value == gpiod::line::value::ACTIVE) {
                    std::cout << "GPIO = 1:\n";
                } else {
                    std::cout << "GPIO = 0: obstacle\n";
                }

                previousValue = value;
            }

            std::this_thread::sleep_for(std::chrono::milliseconds(50));
        }

    } catch (const std::exception& error) {
        std::cerr << "error GPIO: " << error.what() << '\n';
        return 1;
    }


    return 0;
}