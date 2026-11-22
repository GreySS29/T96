#include <gpiod.hpp>
#include <atomic>
#include <thread>
#include <chrono>
#include <algorithm>
#include "IPwm.h"

class SoftPwm : public IPwm 
{
public:
    SoftPwm(gpiod::line::offset pin, const std::string& chip_path, double freq_hz = 200.0) //freq_hz for collector motors
        : pin_(pin),
          period_(std::chrono::duration_cast<std::chrono::steady_clock::duration>(
                      std::chrono::duration<double>(1.0 / freq_hz))),
          request_(gpiod::chip(chip_path).prepare_request()
              .set_consumer("SoftPwm")
              .add_line_settings(pin_,
                  gpiod::line_settings()
                      .set_direction(gpiod::line::direction::OUTPUT)
                      .set_output_value(gpiod::line::value::INACTIVE))
              .do_request()),
          running_(true),
          thread_([this] { run(); }) {}

    ~SoftPwm() override {
        running_ = false;
        thread_.join();
        try { request_.set_value(pin_, gpiod::line::value::INACTIVE); } catch (...) {}
    }

    void set_duty(float d) override { duty_ = std::clamp(d, 0.0f, 1.0f); }

private:
    void run() {
        using V = gpiod::line::value;
        auto next = std::chrono::steady_clock::now();
        while (running_) {
            const float d = duty_;
            if (d <= 0.0f)      request_.set_value(pin_, V::INACTIVE);
            else if (d >= 1.0f) request_.set_value(pin_, V::ACTIVE);
            else {
                request_.set_value(pin_, V::ACTIVE);
                std::this_thread::sleep_until(next + std::chrono::duration_cast<
                    std::chrono::steady_clock::duration>(period_ * d));
                request_.set_value(pin_, V::INACTIVE);
            }
            next += period_;
            std::this_thread::sleep_until(next);
        }
    }

    gpiod::line::offset pin_;
    std::chrono::steady_clock::duration period_;
    gpiod::line_request request_;
    std::atomic<float> duty_{0.0f};
    std::atomic<bool> running_;
    std::thread thread_;        
};