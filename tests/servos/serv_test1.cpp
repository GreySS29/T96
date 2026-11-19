#include <lgpio.h>
#include <iostream>

int main() {
    int h = lgGpiochipOpen(0);
    if (h < 0) {
        std::cerr << " gpiochip " << h << "\n";
        return 1;
    }

    const int PIN = 26;
    lgGpioClaimOutput(h, 0, PIN, 0);

    for (int i = 0; i < 3; i++) {
        lgTxServo(h, PIN, 600, 50, 0, 0);
        lguSleep(1);
        lgTxServo(h, PIN, 1500, 50, 0, 0);
        lguSleep(1);
        lgTxServo(h, PIN, 2500, 50, 0, 0);
        lguSleep(1);
    }

    lgTxServo(h, PIN, 0, 50, 0, 0);   
    lgGpiochipClose(h);
    return 0;
}