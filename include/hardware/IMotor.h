#pragma once
#include <chrono>
#include <thread>

enum class MotorState {ActiveUp, Inactive, ActiveBack, SpeedMode, Brake};

class IMotor
{
public:
    virtual ~IMotor() = default;
    virtual void set_state(MotorState s) = 0;
    virtual void set_speed(float s) = 0;
    virtual void brake() = 0;                 // short brake: IN1 = IN2 = HIGH
    virtual void coast() = 0;              // Hi-Z: IN1 = IN2 = LOW, PWM = HIGH
    virtual MotorState get_state() const noexcept = 0;

protected:
    IMotor() = default;
    IMotor(const IMotor&) = default;       
    IMotor& operator=(const IMotor&) = default;
};