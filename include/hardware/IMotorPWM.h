#pragma once
#include <chrono>
#include <thread>
#include "IMotor.h"


class IMotorPWM : public IMotor
{
public:
    virtual void set_speed(float s) = 0;
    virtual void brake() = 0;                 // short brake: IN1 = IN2 = HIGH
    virtual void coast() = 0;              // Hi-Z: IN1 = IN2 = LOW, PWM = HIGH
};