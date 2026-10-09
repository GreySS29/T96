#pragma once
#include <optional>

enum class SensorState { Ready, Busy, Error };

class ISensor
{

    public:
    virtual ~ISensor() = default;
    virtual std::optional<double> read_distance_cm() = 0;
    virtual SensorState get_state() const noexcept =0;
    
    
   

    protected:
    ISensor() = default;
    ISensor(const ISensor&) = delete;
    ISensor& operator=(const ISensor&) = delete;
  
};

