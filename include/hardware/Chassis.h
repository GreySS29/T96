#pragma once 
#include "IMotor.h"
#include <memory>
#include <thread>
#include <mutex>


class Chassis {
        private:
        std::unique_ptr<IMotor> leftMot_;
        std::unique_ptr<IMotor> rightMot_;
        std::atomic<bool> emergency_stop_{false};
        std::mutex mutex_;

        void forward_imp () {
                leftMot_->set_state(MotorState::ActiveUp);
                rightMot_->set_state(MotorState::ActiveUp);
        }

        void back_imp () {
                leftMot_->set_state(MotorState::ActiveBack);
                rightMot_->set_state(MotorState::ActiveBack);

        }
        void turn_left_full_imp () {
                leftMot_->set_state(MotorState::ActiveUp);
                rightMot_->set_state(MotorState::ActiveBack);
        }

        

        void stop_imp () {
                leftMot_->set_state(MotorState::Inactive);
                rightMot_->set_state(MotorState::Inactive);
        }

        void turn_right_full_imp () {
                leftMot_->set_state(MotorState::ActiveBack);
                rightMot_->set_state(MotorState::ActiveUp);
        }

    public:
        Chassis(std::unique_ptr<IMotor> left, std::unique_ptr <IMotor> right) : 
            leftMot_(std::move(left)) , rightMot_(std::move(right))
    {}

        void set_emergency_stop(bool enabled) {
                emergency_stop_.store(enabled);

                if(enabled) {
                        stop();
                }
        }

        bool is_emergency_stop() const {
                return emergency_stop_.load();
        }

        void forward() {
                std::scoped_lock lock(mutex_);
                if(emergency_stop_.load()){
                        return;  //canceling
                }

                forward_imp();
        }

         void turn_right_full() {
                std::scoped_lock lock(mutex_);
                if(emergency_stop_.load()){
                        return;  
                }

                turn_right_full_imp();
        }

         void turn_left_full() {
                std::scoped_lock lock(mutex_);
                if(emergency_stop_.load()){
                        return;  
                }

                turn_left_full_imp();
        }

        void stop() {
                std::scoped lock lock(mutex_);
                stop_imp();
        }

        void back() {
                std::scoped lock lock(mutex_);
                //work anytime 
                back_imp();
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

        void turn_left_full (auto seconds) {
                leftMot_->set_state(MotorState::ActiveUp);
                rightMot_->set_state(MotorState::ActiveBack);
                std::this_thread::sleep_for(std::chrono::seconds(seconds));
                stop();
        }

        
        
        void turn_right_full (auto seconds) {
                leftMot_->set_state(MotorState::ActiveBack);
                rightMot_->set_state(MotorState::ActiveUp);
                std::this_thread::sleep_for(std::chrono::seconds(seconds));
                stop();
        }
        void turn_left_half () {
                leftMot_->set_state(MotorState::ActiveUp);
                rightMot_->set_state(MotorState::Inactive);
        }

        void turn_right_half () {
                leftMot_->set_state(MotorState::Inactive);
                rightMot_->set_state(MotorState::ActiveUp);
        }

        

};
                



