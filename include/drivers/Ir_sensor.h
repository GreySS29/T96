#pragma once

#include "../hardware/ISensor.h"

#include <gpiod.hpp>

#include <optional>
#include <stdexcept>
#include <string>

class Ir_sensor final : public ISensor // separate interface it is not distant sensor
{
private:
    gpiod::line::offset sensor_pin_;

    gpiod::chip chip_;
    gpiod::line_request request_;

    SensorState state_ = SensorState::Ready;

public:
    explicit Ir_sensor(
        gpiod::line::offset sensor_pin,
        const std::string& chip_path = "/dev/gpiochip0")
        : sensor_pin_(sensor_pin),
          chip_(chip_path),
          request_(
              chip_.prepare_request()
                  .set_consumer("ir-obstacle-sensor")
                  .add_line_settings(
                      sensor_pin_,
                      gpiod::line_settings()
                          .set_direction(gpiod::line::direction::INPUT)
                  )
                  .do_request()
          )
    {
    }

    std::optional<double> read_distance_cm() override { return std::nullopt; }
    bool check_min_distant_cm(double) override { return false; }


    bool is_object_detected() override
    {
        try {
            const auto value = request_.get_value(sensor_pin_);

            state_ = SensorState::Ready;

            return value == gpiod::line::value::INACTIVE;

        } catch (const std::exception&) {
            state_ = SensorState::Error;
            return false;
        }
    }



    SensorState get_state() const noexcept override
    {
        return state_;
    }
};