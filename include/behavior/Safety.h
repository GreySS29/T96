#include "../hardware/Chassis.h"
#include "../hardware/ISensor.h"

class Safety
{
private:
    std::unique_ptr<Chassis>& base_;
    std::unique_ptr<ISensor>& ultraSonicFront_;
    //std::unique_ptr<ISensor>& ultraSonicBack_;
public:
    Safety(std::unique_ptr<Chassis>& base,std::unique_ptr<ISensor>& ultraSonicFront) :
    base_(base), ultraSonicFront_(ultraSonicFront) {}
    void active(){
        // if(ultraSonicFront_->check_min_distant_cm(20.)&& ultraSonicBack_->check_min_distant_cm(20.))
        // {
        //     base_->turn_left_full(1);
        //     base_->forward(2);
        //     base_->turn_right_full(1);
        // }
        if(ultraSonicFront_->check_min_distant_cm(20.)) base_->back(1);
        // if(ultraSonicBack_->check_min_distant_cm(20.)) base_->forward(1);
    }


};

