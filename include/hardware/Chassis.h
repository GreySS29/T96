#pragma once 
#include "IMotor.h"
#include <memory>
#include <thread>

class Chassis {
    private:
    std::unique_ptr<IMotor> leftMot_;
    std::unique_ptr<IMotor> rightMot_;

    public:
        Chassis(std::unique_ptr<IMotor> left, std::unique_ptr <IMotor> right) : 
            leftMot_(std::move(left)) , rightMot_(std::move(right))
    {}
        void forward () {
                leftMot_->set_state(MotorState::ActiveUp);
                rightMot_->set_state(MotorState::ActiveUp);
        }
         void forward (auto seconds) {
                leftMot_->set_state(MotorState::ActiveUp);
                rightMot_->set_state(MotorState::ActiveUp);
                std::this_thread::sleep_for(std::chrono::seconds(seconds));
                stop();
        }

        void back (auto seconds) {
                leftMot_->set_state(MotorState::ActiveBack);
                rightMot_->set_state(MotorState::ActiveBack);
                std::this_thread::sleep_for(std::chrono::seconds(seconds));
                stop();
        }

        void back () {
                leftMot_->set_state(MotorState::ActiveBack);
                rightMot_->set_state(MotorState::ActiveBack);
        
        }

        void turn_left_full (auto seconds) {
                leftMot_->set_state(MotorState::ActiveUp);
                rightMot_->set_state(MotorState::ActiveBack);
                std::this_thread::sleep_for(std::chrono::seconds(seconds));
                stop();
        }
        void turn_left_full () {
                leftMot_->set_state(MotorState::ActiveUp);
                rightMot_->set_state(MotorState::ActiveBack);
        }

        void turn_left_half () {
                leftMot_->set_state(MotorState::ActiveUp);
                rightMot_->set_state(MotorState::Inactive);
        }

        void turn_right_full () {
                leftMot_->set_state(MotorState::ActiveBack);
                rightMot_->set_state(MotorState::ActiveUp);
        }
        void turn_right_full (auto seconds) {
                leftMot_->set_state(MotorState::ActiveBack);
                rightMot_->set_state(MotorState::ActiveUp);
                std::this_thread::sleep_for(std::chrono::seconds(seconds));
                stop();
        }

        void turn_right_half () {
                leftMot_->set_state(MotorState::Inactive);
                rightMot_->set_state(MotorState::ActiveUp);
        }

        void stop () {
                leftMot_->set_state(MotorState::Inactive);
                rightMot_->set_state(MotorState::Inactive);
                }

};
                



