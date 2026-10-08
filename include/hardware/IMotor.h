#pragma once

enum class MotorState { ActiveUp, Inactive, ActiveBack };

class IMotor
{
public:
    virtual ~IMotor() = default;
    virtual void set_state(MotorState s) = 0;
    virtual MotorState get_state() const noexcept = 0;

protected:
    IMotor() = default;
    IMotor(const IMotor&) = default;       
    IMotor& operator=(const IMotor&) = default;
};