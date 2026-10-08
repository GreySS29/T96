#pragma once 
#include "Ymotor.h"
#include <memory>

class Chassis {
    private:
    std::unique_ptr<Ymotor> leftMot_;
    std::unique_ptr<Ymotor> rightMot_;

    public:
        Chassis(std::unique_ptr<Ymotor> left, std::unique_ptr <Ymotor> right) : 
            leftMot_(std::move(left)) , rightMot_(std::move(right))
    {}
        void forward () {
                leftMot_->set_state(MotorState::ActiveUp);
                rightMot_->set_state(MotorState::ActiveUp);
        }

        void back () {
                leftMot_->set_state(MotorState::ActiveBack);
                rightMot_->set_state(MotorState::ActiveBack);
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

        void turn_right_half () {
                leftMot_->set_state(MotorState::Inactive);
                rightMot_->set_state(MotorState::ActiveUp);
        }

        void stop () {
                leftMot_->set_state(MotorState::Inactive);
                rightMot_->set_state(MotorState::Inactive);
                }

};
                



