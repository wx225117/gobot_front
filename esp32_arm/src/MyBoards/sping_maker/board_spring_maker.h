#pragma once
#include "../board_base.h"
#include "MyBoards/cnc_board_base.h"

#include "ESP32Step/src/TeensyStep.h"
#include "CNC/single_axis_homer.h"
#include "MyBoards/gobot_main/board_pins/board_ver1.2.h"
#include "robot_eef/board_spring_maker.h"

class Board_SpringMaker: public CncBoardBase{
    public:
        Board_SpringMaker(){};
        void Init(bool is_on_reset) override;
        
        // void SayHello();
        // Stepper* GetStepper(char axis_name) override;
        SingleAxisHomer* GetHomer(char axis_name) override;
        RobotEef_SpringMaker* GetEef() override;
        void EnableMotor(char axis_name, bool enable_it) override;
        // void AllMotorsMoveTo(bool is_absolute_position, float* motor_position, int motors_count) override{};
        // void SingleMotorMoveTo(char motor_name, float motor_position) override{};         
        // float GetDistanceToTarget() override{};
        // void AllMotorsMoveTo(bool is_absolute_position, float* motor_position, int motors_count) override{};
        // void AllMotorStop() override {};
        // void SingleMotorMoveTo(char motor_name, float motor_position) override{};         
        // float GetDistanceToTarget() override{return 0;};

        // float GetMotorPosition(char motor_name) override {};
        // void SetMotorPosition(char motor_name, float as_current_position) override {};
        
    private:
        Stepper stepper_alpha = Stepper(PIN_ALPHA_STEP_2201, PIN_ALPHA_DIR_2201);
        SingleAxisHomer homer_alpha = SingleAxisHomer(PIN_HOME_ALPHA_2201, LOW);
        RobotEef_SpringMaker eef = RobotEef_SpringMaker();
};
