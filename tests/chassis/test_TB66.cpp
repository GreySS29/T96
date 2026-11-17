#include <gpiod.hpp>
#include <chrono>
#include <iostream>
#include <thread>

using gpiod::line::value;
using namespace std::chrono;

int main() {
    const unsigned AIN1 = 17, AIN2 = 27, PWMA = 22, STBY = 26;
    //const int duty = 90;            // скважность, %
    //const int period_us = 2000;     // период ШИМ 2 мс (~500 Гц)

    auto req = gpiod::chip("/dev/gpiochip0")
        .prepare_request()
        .set_consumer("tb6612")
        .add_line_settings(
            gpiod::line::offsets{AIN1, AIN2, PWMA, STBY},
            gpiod::line_settings()
                .set_direction(gpiod::line::direction::OUTPUT)
                .set_output_value(value::INACTIVE))
        .do_request();

    req.set_value(STBY, value::ACTIVE);   // включить драйвер

    // ---- Вперёд 3 с ----
    std::cout << "Вперёд 50%\n";
    req.set_value(AIN1, value::ACTIVE);
    req.set_value(AIN2, value::INACTIVE);
    auto end = steady_clock::now() + seconds(10);
    while (steady_clock::now() < end) {
        req.set_value(PWMA, value::ACTIVE);
      //  std::this_thread::sleep_for(microseconds(period_us * duty / 100));
        ///req.set_value(PWMA, value::INACTIVE);
       // std::this_thread::sleep_for(microseconds(period_us * (100 - duty) / 100));
    }


    // ---- Пауза 1 с ----
//    std::cout << "Стоп\n";
  //  req.set_value(AIN1, value::INACTIVE);
    //req.set_value(AIN2, value::INACTIVE);
   // std::this_thread::sleep_for(seconds(1));

    // ---- Назад 3 с ----
   // std::cout << "Назад 50%\n";
   // req.set_value(AIN1, value::INACTIVE);
    //req.set_value(AIN2, value::ACTIVE);
    //end = steady_clock::now() + seconds(3);
    //while (steady_clock::now() < end) {
      //  req.set_value(PWMA, value::ACTIVE);
        //std::this_thread::sleep_for(microseconds(period_us * duty / 100));
        //req.set_value(PWMA, value::INACTIVE);
        //std::this_thread::sleep_for(microseconds(period_us * (100 - duty) / 100));
    //}

    // ---- Выключение ----
    std::cout << "Стоп\n";
    req.set_value(AIN1, value::INACTIVE);
    req.set_value(AIN2, value::INACTIVE);
    req.set_value(PWMA, value::INACTIVE);
    req.set_value(STBY, value::INACTIVE);

    return 0;
}
