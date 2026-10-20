#include <memory>
#include "../hardware/Chassis.h"

class BypassingObstacle
{
private:
   Chassis& chassis_;
public:
    BypassingObstacle(Chassis& chassis) :
        chassis_(chassis){};
    void run () {
        chassis_.turn_left_full(1);
        chassis_.forward(1);
        chassis_.turn_right_full(1);
        chassis_.forward(1);
        chassis_.turn_left_full(1);
        chassis_.forward(1);
        chassis_.turn_right_full(1);
}
};


