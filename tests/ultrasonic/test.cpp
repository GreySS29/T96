#include <gpiod.hpp>
#include <chrono>
#include <iostream>
#include <stdexcept>
#include <thread>

using namespace std::chrono;

int main() {
    const std::string chip_name = "/dev/gpiochip0";
    const gpiod::line::offset trig_offset = 5;  
    const gpiod::line::offset echo_offset = 6;  

    gpiod::chip chip(chip_name);

    auto request = chip.prepare_request()
        .set_consumer("hc-sr04")
        .add_line_settings(
            trig_offset,
            gpiod::line_settings()
                .set_direction(gpiod::line::direction::OUTPUT)
                .set_output_value(gpiod::line::value::INACTIVE)
        )
        .add_line_settings(
            echo_offset,
            gpiod::line_settings()
                .set_direction(gpiod::line::direction::INPUT)
        )
        .do_request();

    while (true) {
        request.set_value(trig_offset, gpiod::line::value::INACTIVE);
        std::this_thread::sleep_for(microseconds(5));

        request.set_value(trig_offset, gpiod::line::value::ACTIVE);
        std::this_thread::sleep_for(microseconds(10));
        request.set_value(trig_offset, gpiod::line::value::INACTIVE);

        const auto timeout = milliseconds(30);

        auto start = steady_clock::now();
        while (request.get_value(echo_offset) == gpiod::line::value::INACTIVE) {
            if (steady_clock::now() - start > timeout) {
                std::cout << "timeout: no echo\n";
                break;
            }
        }

        start = steady_clock::now();
        while (request.get_value(echo_offset) == gpiod::line::value::ACTIVE) {
            if (steady_clock::now() - start > timeout) {
                std::cout << "timeout: echo stuck high\n";
                break;
            }
        }

        const auto pulse = steady_clock::now() - start;
        const double seconds =
            static_cast<double>(
                duration_cast<nanoseconds>(pulse).count()) / 1e9;

        const double distance_cm = seconds * 34300.0 / 2.0;

        if (distance_cm > 0.0 && distance_cm < 450.0) {
            std::cout << distance_cm << " cm\n";
        }

        std::this_thread::sleep_for(milliseconds(200));
    }
}
