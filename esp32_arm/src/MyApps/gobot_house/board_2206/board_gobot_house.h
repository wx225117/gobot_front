#pragma once
#include "CNC/board/cnc_board_base.h"
#include "../board_2205/board_pins/board_pins_ver3.11.h"
#include "../board_2205/robot_eef/gobot_house_eef.h"
#include "CNC/Actuator/servo/cnc_actuator_servo.h"
#include "CNC/Actuator/stepper/actuator_stepper.h"
#include "ESP32Step/src/TeensyStep.h" 
#include "Robot/axis_homer/position_trigger_array.h"

class Board_GobotHouse_2206: public CncBoardBase{
    public:
        Board_GobotHouse_2206(StepControl* stepControl){this->__stepControl=stepControl;};
        Board_GobotHouse_2206(){};
        void Init(bool is_on_reset) override;
        // CncActuatorBase* GetActuator(EnumAxis axis_name) override;
        CncActuatorServo* GetActuaorBeta(char axis_name);
        // MotorBase* GetActuator(char axis_name){};  //TODO: Stepper, Servo, BLDC... all is motorbase!
        // PositionTrigger* GetSingleHomer(EnumAxis axis_name) override;
        // AxisHomer* GetPositionTriggers(EnumAxis axis_name) override;

        // RobotEef_GobotHouse* GetEef() override;

        void EnableMotor(EnumAxis_Inverseinematic axis_name, bool enable_it) override;
        Servo* GetServoDriver_OnBeta(){return &this->__servo_beta;};
        Stepper* GetStepperDriver_OnAlpha(){return &this->__alpha_stepper;};
    
    private:
        void __Init_Hardware();
        
        Stepper __alpha_stepper = Stepper(PIN_ALPHA_STEP_2109, PIN_ALPHA_DIR_2109);
        CncActuatorStepper __actuator_alpha;
        ActuatorRangeConstraintBase __alpha_range;
        Servo __servo_beta;
        CncActuatorServo __actuator_beta;
        // CncMover_StepperServo mover_StepperServo;

        // PositionTrigger* GetPositionTrigger(uint8_t index) override{return nullptr;};

        PositionTrigger __all_position_triggers[2];
        RobotEef_GobotHouse eef;
        // GobotHouse_CncSolutionConfig_2206 __cnc_mechanic;

        StepControl* __stepControl;
};
