#pragma once
#include "../hardware/ISensor.h"
#include "drivers/HC_SR04.h"
#include "drivers/Ir_sensor.h"

class SensorPanel
{
private:
 
public:
    std::unique_ptr<ISensor> ultraSonicFront_;
    std::unique_ptr<ISensor> ir_sensorRight_;
    std::unique_ptr<ISensor> ir_sensorLeft_;
    std::unique_ptr<ISensor> ir_sensorLeftBack_;
    std::unique_ptr<ISensor> ir_sensorRightBack_;
    

    SensorPanel(
        std::unique_ptr<ISensor> ultraSonicFront , 
        std::unique_ptr<ISensor> ir_sensorRight,
        std::unique_ptr<ISensor>  ir_sensorLeft , 
        std::unique_ptr<ISensor> ir_sensorRightBack,
        std::unique_ptr<ISensor>  ir_sensorLeftBack) :
    ultraSonicFront_ (std::move(ultraSonicFront)),
    ir_sensorRight_(std::move(ir_sensorRight)),
    ir_sensorLeft_(std::move(ir_sensorLeft)),
    ir_sensorLeftBack_(std::move(ir_sensorLeftBack)),
    ir_sensorRightBack_(std::move(ir_sensorRightBack)){};
};
