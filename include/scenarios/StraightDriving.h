#include "../behavior/BypassingObstacle.h"
#include "../hardware/Chassis.h"
#include "../hardware/SensorPanel.h"

#include <memory>
#include <thread>

//strange class now
class StraightDriving
{
private:
   
    Chassis& base_;
    BypassingObstacle bypObs_;
    SensorPanel& sensPanel_;

public:
    StraightDriving( std::unique_ptr<Chassis>& base, std::unique_ptr<SensorPanel>& sensPanel  ) :
        base_(*base),
        bypObs_(*base),
        sensPanel_(*sensPanel)
    {};

    void active(std::chrono::seconds second)
    {
        const auto finish = std::chrono::steady_clock::now() + second;

        while (std::chrono::steady_clock::now() < finish)
        {
            if (sensPanel_.ultraSonicFront_->check_min_distant_cm(20))
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


