#include <gpiod.hpp>
#include <chrono>
#include <iostream>
#include <thread>

using gpiod::line::value;

int main() {
    const unsigned AIN1 = 17, AIN2 = 27, PWMA = 22,
	BIN1 =23 , BIN2=24, PWMB = 25, STBY = 26;

    try {
        auto req = gpiod::chip("/dev/gpiochip0")
            .prepare_request()
            .set_consumer("tb6612")
            .add_line_settings(
                gpiod::line::offsets{AIN1, AIN2, PWMA,BIN1,BIN2, PWMB, STBY},
                gpiod::line_settings()
                    .set_direction(gpiod::line::direction::OUTPUT)
                    .set_output_value(value::INACTIVE))
            .do_request();

        std::cout << "Вперёд\n";
        req.set_value(AIN1, value::ACTIVE);
        req.set_value(AIN2, value::INACTIVE);
	req.set_value(BIN1, value::ACTIVE);
	req.set_value(BIN2, value::INACTIVE);
        req.set_value(PWMA, value::ACTIVE);
	req.set_value(PWMB, value::ACTIVE);
        req.set_value(STBY, value::ACTIVE);

        std::this_thread::sleep_for(std::chrono::seconds(3));

	std::cout << "Basck\n";
	req.set_value(AIN1, value::INACTIVE);
	req.set_value(AIN2, value::ACTIVE);
	
	std::this_thread::sleep_for(std::chrono::seconds(3));

        std::cout << "Стоп\n";
        req.set_value(PWMA, value::INACTIVE);
	req.set_value(PWMB, value::INACTIVE);
        req.set_value(AIN1, value::INACTIVE);
	req.set_value(AIN2, value::INACTIVE);
	req.set_value(BIN1, value::INACTIVE);
	req.set_value(BIN2, value::INACTIVE);
        req.set_value(STBY, value::INACTIVE);
    } catch (const std::exception& e) {
        std::cerr << "Ошибка: " << e.what() << "\n";
        return 1;
    }
    return 0;
}
