#pragma once
#include <gpiod.h>
#include <unistd.h>


enum class Motor_state {ActiveUp, Inactive , ActiveBack};

class Motor_driver{
    private:
    Motor_state state;
    gpiod_chip* chip = nullptr;
    gpiod_line_settings* settings = nullptr;
    gpiod_line_config* config = nullptr;
    gpiod_line_request* request = nullptr;
    unsigned int pins[2]; // later EAN 
    //PWM
    void cleanup();

    public:
    Motor_driver(int pin1 , int pin2);
    ~Motor_driver() {cleanup();}

    //set
    void set_state(Motor_state new_state);

    //get
    void get_state() const {return state;}

}