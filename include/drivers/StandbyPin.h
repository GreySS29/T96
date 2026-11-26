#include <gpiod.hpp>
#include <string>

class StandbyPin {
    gpiod::line_request req_;
    unsigned int offset_;

public:
    StandbyPin(unsigned int offset, const std::string& chip_path)
        : req_(gpiod::chip(chip_path)
                   .prepare_request()
                   .set_consumer("stby")
                   .add_line_settings(
                       offset,
                       gpiod::line_settings()
                           .set_direction(gpiod::line::direction::OUTPUT)
                           .set_output_value(gpiod::line::value::ACTIVE))
                   .do_request()),
          offset_(offset) {}

    ~StandbyPin() {
        try { req_.set_value(offset_, gpiod::line::value::INACTIVE); }
        catch (...) {}
    }
};