#pragma once
#include "ESP32Step/src/TeensyStep.h"

struct motor_position{
    int alpha;
    int beta;
};

/*
Currently is a 2D arm.
*/
class RobotArm{
    public:
        virtual void Home(unsigned char axis);
        void LinkStepper(Stepper* alpha, Stepper* beta);

    protected:
        void MoveTo(int16_t x, int16_t y);
        virtual motor_position ik(float x, float y);

        Stepper* stepper_alpha;
        Stepper* stepper_beta;
        StepControl* steppers;
    private:

};