T96
![base](doc/version%200.1/PSX_20261014_133745.jpg)

**Topic:** Implementation of an Autonomous Robot Using a Makeblock Kit and Raspberry Pi 4 - v.0.1

### Introduction

This project aims to build an autonomous mobile robot using a Makeblock kit, Raspberry Pi 4, an HC-SR04 ultrasonic distance sensor, and two infrared obstacle-detection sensors.

### Software Architecture

The project is developed in C++ on Linux. 

This approach provides:

- Low coupling between sensors, motor drivers, and behavior logic
- Easy replacement and extension of hardware components
- Memory safety and prevention of resource leaks
- Scalability for future modules such as telemetry, camera vision, and LiDAR

### Hardware Configuration

- Makeblock chassis and motors
- Raspberry Pi 4
- HC-SR04 ultrasonic distance sensor
- Two infrared obstacle-detection sensors
- Motor driver L298N

The HC-SR04 measures the distance to obstacles ahead, while the infrared sensors detect obstacles on the sides of the robot. 
### Completed Work

1. Designed the C++ project architecture using interfaces and `std::unique_ptr`
2. Tested the HC-SR04 and infrared sensors
3. Tested the chassis and motor operation
4. Configured remote SSH access to the Raspberry Pi through a tunnel
5. Implemented keyboard-based motor control over SSH
6. Implemented basic obstacle-avoidance behavior
7. Started developing improved obstacle-bypass behavior

### Remote Control and Debugging

An SSH tunnel allows remote access to the Raspberry Pi without a monitor or keyboard. This makes it possible to modify code, run programs, test sensors, and control motors remotely.

### Obstacle Avoidance

The current basic behavior works as follows:

- Move forward when no obstacle is detected
- Stop when an obstacle is detected
- Choose a maneuver based on sensor data: turn left, turn right, or move backward
- Continue moving forward once the path is clear

This is a common sensor-based approach used in mobile robots.

### Current Work

The project is currently focused on improving obstacle bypassing. Instead of only stopping or turning, the robot will use distance measurements and left/right infrared sensor data to select a safer and smoother route.

### Future Development Plans

- Install a camera for vision-based recognition
- Introduce LiDAR in the future for more accurate environment perception
- Implement a state-machine-based control system
- Add telemetry and logging features
- Develop autonomous navigation using camera and LiDAR data
