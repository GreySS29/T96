#pragma once
#include "../hardware/ISensor.h"
#include <gpiod.hpp>

#include <chrono>
#include <cstdint>
#include <optional>
#include <stdexcept>
#include <string>
#include <thread>

class HC_SR04 final : public ISensor
{

    private:
    static constexpr double kMinCm = 2.;
    static constexpr double kMaxCm = 100.;
    static constexpr double kSoundHalf = 17150.;  
    static constexpr auto   kTimeout = std::chrono::milliseconds(40);

    std::optional<double> measure();
    
    gpiod::line::offset trig_;  
    gpiod::line::offset echo_;
    gpiod::line_request req_;

    SensorState state_ = SensorState::Ready;

    public:
        explicit HC_SR04(gpiod::line::offset trig,
                        gpiod::line::offset echo,
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
                    .do_request())
        {
            if (trig == echo) {
                throw std::invalid_argument("trig and echo pins must differ");
            }
        }

        std::optional<double> read_distance_cm() override;

        SensorState get_state() const noexcept override { return state_; }


};