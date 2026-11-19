#include <fstream>
#include <iostream>
#include <string>
#include <thread>
#include <chrono>
#include <cmath>

const std::string CHIP = "/sys/class/pwm/pwmchip0/";

const int MIN_US = 500;      // 0°
const int MAX_US = 2500;     // 180°
const double STEP_DEG = 1.0;
const int DELAY_MS = 30;

void writeFile(const std::string& path, const std::string& value) {
    std::ofstream f(path);
    if (!f) { std::cerr << "err" << path << "\n"; return; }
    f << value;
}

struct Servo {
    std::string channel;      
    std::string path;
    double angle = 90.0;

    explicit Servo(const std::string& ch) : channel(ch), path(CHIP + "pwm" + ch + "/") {}

    void init() {
        writeFile(CHIP + "export", channel);
        std::this_thread::sleep_for(std::chrono::milliseconds(200));
        writeFile(path + "period", "20000000");   // 50 hz
        set(90);
        writeFile(path + "enable", "1");
    }

    void set(double a) {
        if (a < 0) a = 0;
        if (a > 180) a = 180;
        int us = MIN_US + (int)(a * (MAX_US - MIN_US) / 180.0);
        writeFile(path + "duty_cycle", std::to_string(us * 1000));  // ns
        angle = a;
    }

    void stop() {
        writeFile(path + "enable", "0");
        writeFile(CHIP + "unexport", channel);
    }
};


void stepToward(Servo& s, double target) {
    if (s.angle < target) s.set(std::min(s.angle + STEP_DEG, target));
    else if (s.angle > target) s.set(std::max(s.angle - STEP_DEG, target));
}


void moveBoth(Servo& a, double ta, Servo& b, double tb) {
    ta = std::max(0.0, std::min(180.0, ta));
    tb = std::max(0.0, std::min(180.0, tb));
    while (a.angle != ta || b.angle != tb) {
        stepToward(a, ta);
        stepToward(b, tb);
        std::this_thread::sleep_for(std::chrono::milliseconds(DELAY_MS));
    }
}

int main() {
    Servo s1("0");   // GPIO18
    Servo s2("1");   // GPIO19
    s1.init();
    s2.init();
    std::this_thread::sleep_for(std::chrono::seconds(1));

    std::cout << "0-180 with space\n";
    while (true) {
        double a, b;
        std::cout << "Angle (first second): ";
        if (!(std::cin >> a >> b)) break;
        moveBoth(s1, a, s2, b);
    }

    s1.stop();
    s2.stop();
    return 0;
}