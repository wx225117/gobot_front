#pragma once

#include "../actuator_base.h"
#include <ESP32Servo.h>


class ActuatorServo: public ActuatorBase{
    public:
        void LinkServo(Servo* servo, bool is_inversed_dir);
        void SetInverseDir(bool inversed){this->__inversed_dir=inversed;};

        float GetCurrentPosition_InCncUnit() override;
        void SetPosition(float cnc_position_in_rad) override;
        void MoveTo(bool is_absolute_position, float position_in_cnc_unit) override;
        float GetDistanceToTarget_InCncUnit() override;
        void SetSpeed(float speed_per_second) override;
        // bool MotorIsMoving(char moto_name) override;
        void SpinOnce();


    protected:

    private:
        Servo* __servo;
        float __current_position_in_rad;

        bool __inversed_dir;
        float __position_offset_in_rad = 0;   // Logical position - physic position
        float __ToServoDegree(float from_cnc_rad);
        float __ToCncRad(float from_servo_angle);

        float __speed_degree_per_second;
        int64_t last_spin_timestamp;
        uint8_t __moving_direction;
        bool __is_moving=false;
        float __distance_to_target_in_rad;
};