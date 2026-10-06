#include <gpiod.hpp>
#include <algorithm>
#include <chrono>
#include <iostream>
#include <stdexcept>
#include <thread>
#include <vector>

using namespace std::chrono;

static double measure_once(
    gpiod::line_request& request,
    gpiod::line::offset trig,
    gpiod::line::offset echo
) {
    request.set_value(trig, gpiod::line::value::INACTIVE);
    std::this_thread::sleep_for(microseconds(5));

    request.set_value(trig, gpiod::line::value::ACTIVE);
    std::this_thread::sleep_for(microseconds(10));
    request.set_value(trig, gpiod::line::value::INACTIVE);

    const auto timeout = milliseconds(40);

    gpiod::edge_event rising;
    gpiod::edge_event falling;

    auto wait_for = [&](gpiod::edge_event& event,
                        gpiod::edge_event::event_type type) {
        auto deadline = steady_clock::now() + timeout;

        while (steady_clock::now() < deadline) {
            if (request.read_edge_event(event, microseconds(1000))) {
                if (event.type() == type) {
                    return true;
                }
            }
        }
        return false;
    };

    if (!wait_for(rising, gpiod::edge_event::event_type::RISING_EDGE)) {
        throw std::runtime_error("timeout waiting for rising edge");
    }

    const auto pulse_start = steady_clock::now();

    if (!wait_for(falling, gpiod::edge_event::event_type::FALLING_EDGE)) {
        throw std::runtime_error("timeout waiting for falling edge");
    }

    const auto pulse_end = steady_clock::now();

    const auto pulse = pulse_end - pulse_start;
    const double seconds =
        static_cast<double>(
            duration_cast<nanoseconds>(pulse).count()) / 1e9;

    return seconds * 34300.0 / 2.0;
}

static double median(std::vector<double> values) {
    std::sort(values.begin(), values.end());
    return values[values.size() / 2];
}

int main() {
    const std::string chip_path = "/dev/gpiochip0";
    const gpiod::line::offset trig_offset = 5;
    const gpiod::line::offset echo_offset = 6;

    gpiod::chip chip(chip_path);

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
                .set_edge_detection(gpiod::line::edge::BOTH)
        )
        .do_request();

    while (true) {
        std::vector<double> valid;

        for (int i = 0; i < 8 && valid.size() < 5; ++i) {
            try {
                const double distance = measure_once(
                    request, trig_offset, echo_offset);

                if (distance >= 2.0 && distance <= 400.0) {
                    valid.push_back(distance);
                }
            } catch (const std::exception& e) {
                std::cerr << e.what() << '\n';
            }

            std::this_thread::sleep_for(milliseconds(60));
        }

        if (!valid.empty()) {
            std::cout << median(valid) << " cm\n";
        } else {
            std::cout << "no valid measurement\n";
        }
    }
}