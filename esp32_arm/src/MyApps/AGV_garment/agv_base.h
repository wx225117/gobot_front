#pragma once

#include "SimpleFOC/common/pid.h"
#include "Robot/Actuator/ActuatorBase.h"
// #include "Robot/Actuator/DCMotor/h_bridge_l298n.h"
#include "track_sensor_base.h"

enum AGV_DIRECTION{
    FORWARD = 1,
    BACKWARD = 2,
};

class AgvBase{
    public:
        AgvBase();
        // void Init(PIDController* wheel_pid, L298N* left_wheel, L298N* right_wheel);
        virtual void Move(AGV_DIRECTION direction, int speed_in_percent){};
        virtual void Stop(){};
        // virtual int ReadTrackSensor();
        void SpinOnce();
        float common_speed = 0.0f;
        PIDController* speed_pid;
        void LinkLineSensor(TrackSensorBase* lineSensor){this->lineSensor = lineSensor;};

        TrackSensorBase* lineSensor;
    protected:
        virtual void _SpinOnce_Enter(){};
        virtual void _SpinOnce_Exit(){};

    private:


};