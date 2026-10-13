#include <gpiod.hpp>

#include <algorithm>
#include <chrono>
#include <cstdint>
#include <iostream>
#include <stdexcept>
#include <thread>
#include <vector>

using namespace std::chrono;
using gpiod::line::value;

const gpiod::line::offset TRIG = 5; 
const gpiod::line::offset ECHO = 6;  

double measure(gpiod::line_request& req) {
    gpiod::edge_event_buffer buf(16);

    // remove old events
    while (req.wait_edge_events(nanoseconds(0)))
        req.read_edge_events(buf, 16);

    
    req.set_value(TRIG, value::ACTIVE);
    std::this_thread::sleep_for(microseconds(10));
    req.set_value(TRIG, value::INACTIVE);


    std::uint64_t rise = 0;
    while (req.wait_edge_events(milliseconds(40))) {
        const auto n = req.read_edge_events(buf, 16);
        for (std::size_t i = 0; i < n; ++i) {
            const auto& ev = buf.get_event(i);
            if (ev.type() == gpiod::edge_event::event_type::RISING_EDGE) {
                rise = ev.timestamp_ns();
            } else if (rise != 0) {
                const double sec = (ev.timestamp_ns() - rise) / 1e9;
                return sec * 17150.0;  // 34300 sm/s / 2
            }
        }
    }
    throw std::runtime_error("timeout");
}

int main() {
    try {
        gpiod::chip chip("/dev/gpiochip0");  

        auto req = chip.prepare_request()
            .set_consumer("hc-sr04")
            .add_line_settings(TRIG, gpiod::line_settings()
                .set_direction(gpiod::line::direction::OUTPUT)
                .set_output_value(value::INACTIVE))
            .add_line_settings(ECHO, gpiod::line_settings()
                .set_direction(gpiod::line::direction::INPUT)
                .set_edge_detection(gpiod::line::edge::BOTH))
            .do_request();

        while (true) {
            
            std::vector<double> d;
            for (int i = 0; i < 5; ++i) {
                try {
                    d.push_back(measure(req));
                } catch (const std::exception&) {
                    
                }
                std::this_thread::sleep_for(milliseconds(60));
            }

            if (d.empty()) {
                std::cout << "Echo does't return " << std::endl;
            } else {
                std::sort(d.begin(), d.end());
                std::cout << d[d.size() / 2] << " sm" << std::endl;
            }
        }
    } catch (const std::exception& e) {
        std::cerr << "error: " << e.what() << '\n';
        return 1;
    }
}
