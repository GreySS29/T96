

class IPwm {
public:
    virtual ~IPwm() = default;
    virtual void set_duty(float duty) = 0;   
};