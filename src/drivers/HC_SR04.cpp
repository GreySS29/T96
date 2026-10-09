#include "../../include/drivers/HC_SR04.h"

std::optional<double> HC_SR04::measure()
{
    using namespace std::chrono;
    using V = gpiod::line::value;
    gpiod::edge_event_buffer buf(16);

    // refresh
    while (req_.wait_edge_events(nanoseconds(0))) {
        req_.read_edge_events(buf, 16);
    }

    // impulse
    req_.set_value(trig_, V::ACTIVE);
    std::this_thread::sleep_for(microseconds(10));
    req_.set_value(trig_, V::INACTIVE);

    
    const auto deadline = steady_clock::now() + kTimeout;
    std::uint64_t rise = 0;

    for (;;) {
        const auto left = deadline - steady_clock::now();
        if (left <= nanoseconds(0)) return std::nullopt;
        if (!req_.wait_edge_events(duration_cast<nanoseconds>(left))) return std::nullopt;

        const auto n = req_.read_edge_events(buf, 16);
        for (std::size_t i = 0; i < n; ++i) {
            const auto& ev = buf.get_event(i);
            if (ev.line_offset() != echo_) continue;

            if (ev.type() == gpiod::edge_event::event_type::RISING_EDGE) {
                rise = ev.timestamp_ns();
            } else if (rise != 0) {
                const auto dt_ns = ev.timestamp_ns() - rise;
                return static_cast<double>(dt_ns) * 1e-9 * kSoundHalf;
            }
        }
    }
}

std::optional<double> HC_SR04::read_distance_cm() 
{
    const auto d = measure();
    if (!d || *d < kMinCm || *d > kMaxCm) {
        state_ = SensorState::Error;
        return std::nullopt;
    }
    state_ = SensorState::Ready;
    return d;
}
