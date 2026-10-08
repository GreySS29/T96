
#include <gpiod.h>


class Ysensor
{

    public:
    virtual ~Ysensor();
    Ysensor(const Ysensor&) = delete;
    Ysensor& operator=(const Ymotor&) = delete;

    
   

    protected:
    Ysensor() = default;
    void cleanup();
  
    gpiod::line::offset* trig_ = nullptr;
    gpiod::line::offset* echo_ = nullptr;
    gpiod_chip* chip_ = nullptr;
    // enum class sensorState? 
};

Ysensor::Ysensor(/* args */)
{
}

Ysensor::~Ysensor()
{
}
