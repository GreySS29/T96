#include <lgpio.h>
#include <iostream>

const int PIN = 26;


const int MIN_US = 500;    //  0°
const int MAX_US = 2500;   //  180°

const double STEP_DEG = 1.0;     
const double DELAY_S  = 0.03;    

int handle;
double currentAngle = 90.0;


int angleToUs(double angle) {
    return MIN_US + (int)(angle * (MAX_US - MIN_US) / 180.0);
}


void setAngle(double angle) {
    if (angle < 0) angle = 0;
    if (angle > 180) angle = 180;
    lgTxServo(handle, PIN, angleToUs(angle), 50, 0, 0);
    currentAngle = angle;
}


void moveTo(double target) {
    if (target < 0) target = 0;
    if (target > 180) target = 180;

    while (currentAngle != target) {
        if (currentAngle < target) {
            currentAngle += STEP_DEG;
            if (currentAngle > target) currentAngle = target;
        } else {
            currentAngle -= STEP_DEG;
            if (currentAngle < target) currentAngle = target;
        }
        setAngle(currentAngle);
        lguSleep(DELAY_S);
    }
}

int main() {
    handle = lgGpiochipOpen(0);   
    if (handle < 0) {
        std::cerr << "gpiochip" << handle << "\n";
        return 1;
    }
    lgGpioClaimOutput(handle, 0, PIN, 0);

    setAngle(90);                
    lguSleep(1);

    std::cout << " 0-180 ( -1 exit)\n";
    while (true) {
        double a;
        std::cout << "Angle: ";
        if (!(std::cin >> a) || a < 0) break;
        moveTo(a);
    }

    lgTxServo(handle, PIN, 0, 50, 0, 0);   
    lgGpiochipClose(handle);
    return 0;
}