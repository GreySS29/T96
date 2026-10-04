#include <gpiod.h>
#include <unistd.h>
#include <stdexcept>



enum class MotorState {ActiveUp, Inactive , ActiveBack};

class Ymotor{
 
    public:
    virtual ~Ymotor();
    virtual void set_state(MotorState new_state) = 0;
    virtual MotorState get_state() const noexcept {return state_;}
    Ymotor(const Ymotor&) = delete;
    Ymotor& operator=(const Ymotor&) = delete;
        
    protected:
    Ymotor() = default;
    //void set_state(MotorState new_state) noexcept {state_ = new_state;};
    void cleanup();

    gpiod_chip* chip_ = nullptr;
    gpiod_line_settings* settings_ = nullptr;
    gpiod_line_config* config_ = nullptr;
    gpiod_line_request* request_ = nullptr;
    
    MotorState state_ = MotorState::Inactive;
    unsigned int pins_[2]; // later EAN 
    //PWM
        

};

