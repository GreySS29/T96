#include "../behavior/Bypassing_obstacle.h"
#include "../hardware/Chassis.h"
#include "../hardware/ISensor.h"

#include <memory>
#include <thread>

class StraightDriving
{
private:
    Bypassing_obstacle bypObs_;
    Chassis& base_;
    ISensor& frontSens_;

public:
    StraightDriving( std::unique_ptr<Chassis>& base, std::unique_ptr<ISensor>& frontSens  ) :
        base_(base),
        frontSens_(frontSens)
    {};

    void active(std::chrono::seconds second)
    {
        const auto finish = std::chrono::steady_clock::now() + second;

        while (std::chrono::steady_clock::now() < finish)
        {
            if (frontSens_.check_min_distance_cm(15))
            {
                base_.stop();
                bypObs_.run();
                return;
            }

            base_.forward(1);
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }

        base_.stop();
    }
};


