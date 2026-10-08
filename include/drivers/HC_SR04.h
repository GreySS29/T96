#pragma once
#include <gpiod.hpp>

#include <algorithm>
#include <chrono>
#include <cstdint>
#include <optional>
#include <stdexcept>
#include <string>
#include <thread>
#include <vector>

class HC_SR04 {
public:
    explicit HC_SR04(gpiod::line::offset trig = 5,
                     gpiod::line::offset echo = 6,
                     const std::string& chip_path = "/dev/gpiochip0")
        : trig_(trig), echo_(echo),
          req_(gpiod::chip(chip_path).prepare_request()
                   .set_consumer("hc-sr04")
                   .add_line_settings(trig_, gpiod::line_settings()
                       .set_direction(gpiod::line::direction::OUTPUT)
                       .set_output_value(gpiod::line::value::INACTIVE))
                   .add_line_settings(echo_, gpiod::line_settings()
                       .set_direction(gpiod::line::direction::INPUT)
                       .set_edge_detection(gpiod::line::edge::BOTH))
                   .do_request()) {}

    // Median of 5 readings in cm, or nullopt if the echo never returns.
    std::optional<double> distance_cm() {
        std::vector<double> d;
        for (int i = 0; i < 5; ++i) {
            try { d.push_back(measure()); } catch (const std::exception&) {}
            std::this_thread::sleep_for(std::chrono::milliseconds(60));
        }
        if (d.empty()) return std::nullopt;
        std::sort(d.begin(), d.end());
        return d[d.size() / 2];
    }

private:
    double measure() {
        using namespace std::chrono;
        gpiod::edge_event_buffer buf(16);

        while (req_.wait_edge_events(nanoseconds(0)))
            req_.read_edge_events(buf, 16);

        req_.set_value(trig_, gpiod::line::value::ACTIVE);
        std::this_thread::sleep_for(microseconds(10));
        req_.set_value(trig_, gpiod::line::value::INACTIVE);

        std::uint64_t rise = 0;
        while (req_.wait_edge_events(milliseconds(40))) {
            const auto n = req_.read_edge_events(buf, 16);
            for (unsigned int i = 0; i < n; ++i) {
                const auto& ev = buf.get_event(i);
                if (ev.type() == gpiod::edge_event::event_type::RISING_EDGE) {
                    rise = ev.timestamp_ns();
                } else if (rise != 0) {
                    return static_cast<double>(ev.timestamp_ns() - rise) / 1e9 * 17150.0;
                }
            }
        }
        throw std::runtime_error("timeout");
    }

    gpiod::line::offset trig_;   // declared before req_ so it's initialized first
    gpiod::line::offset echo_;
    gpiod::line_request req_;
};
