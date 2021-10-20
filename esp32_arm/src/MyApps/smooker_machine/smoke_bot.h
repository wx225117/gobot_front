#pragma once

#include "Robot/RobotBase.h"

#include "ESP32Step/src/TeensyStep.h"
#include "Robot/HomeHelper.h"
#include "Robot/Commu/CommuUart.h"
#include "Robot/Actuator/ActuatorBase.h"

#define PIN_HOME 22
#define PIN_STEP 23
#define PIN_DIR 24


class SmokeBot: public RobotBase{
    public:
        SmokeBot();
        void HomeAllAxises() override {};
        void Init_Linkage() override;


    private:
        CommuUart objCommuUart = CommuUart();
        // SingleAxis objAxis_Alpha= SingleAxis();
        ActuatorBase objActuator = ActuatorBase();
        Stepper objStepper = Stepper(PIN_STEP, PIN_DIR);
        StepControl objStepControl;

        HomeHelper objHomeHelper = HomeHelper(PIN_HOME, HIGH);
        
        void MoveToTargetPosition()  override {};
        void SpinOnce_BaseEnter() override {};
        void SpinOnce_BaseExit() override {};
        IkPositionBase* IK(FkPositionBase* fk) override {};
        FkPositionBase* FK(IkPositionBase* ik) override {};
        void RunG1(Gcode* gcode) override;   //None blocking, move backgroundly.
        void RunG6(Gcode* gcode) override {};   //Block mode
        std::string GetHomeTrigerStateString() override {};

};