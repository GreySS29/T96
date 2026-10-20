#include "../behavior/BypassingObstacle.h"
#include "../hardware/Chassis.h"
#include "../hardware/ISensor.h"

#include <memory>
#include <thread>

class StraightDriving
{
private:
   
    Chassis& base_;
    BypassingObstacle bypObs_;
    ISensor& frontSens_;

public:
    StraightDriving( std::unique_ptr<Chassis>& base, std::unique_ptr<ISensor>& frontSens  ) :
        base_(*base),
        bypObs_(*base),
        frontSens_(*frontSens)
    {};

    void active(std::chrono::seconds second)
    {
        const auto finish = std::chrono::steady_clock::now() + second;

        while (std::chrono::steady_clock::now() < finish)
        {
            if (frontSens_.check_min_distant_cm(20))
            {
                base_.stop();
                bypObs_.run();
                return;
            }

            base_.forward();
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }

        base_.stop();
    }
};


