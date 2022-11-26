#include "twh_robot.h"
#include "Robot/mcode_runner/mcode_os.h"
#include "CNC/Actuator/dc_motor/actuator_encoder_calculator.h"



void TeechWarehouse_Robot::Init(TeethWarehouse_Board* board){
    Logger::Debug("TeechWarehouse_Robot::Init()");
    this->_InitStatic_Queues();
    this->__InitStatic_Actuators(board);
    this->_InitStatic_PositionTriggers();
    this->_Init_ArmSolution();

    this->_LinkMover(&__mover);
    
    this->_g28_runner = &this->__g28_runner;
    __g28_runner.Init(&__mover, &__arm_solution);

}

void TeechWarehouse_Robot::__InitStatic_Actuators(TeethWarehouse_Board* board){
    Logger::Info("TeechWarehouse_Robot::Init() Actuators.");
    CncActuator_List::Instance().Init(__all_actuators, CNC_ACTUATORS_COUNT);
    CncActuator_List::Instance().AddActuator(&__actuator_alpha);
    CncActuator_List::Instance().AddActuator(&__actuator_beta);
    CncActuator_List::Instance().AddActuator(&__actuator_gamma);

    this->__actuator_alpha.MyName = 'a';
    this->__actuator_beta.MyName = 'b';
    this->__actuator_gamma.MyName = 'g';
    
    ActuatorStepper_Calculator calculator;
    calculator.motor_step_angle_in_degree = 1.8;
    calculator.motor_driver_micro_steps = 16;
    calculator.motor_gear_teeth_count = 15;
    calculator.motor_gear_pitch_in_mm = 3;
    calculator.slave_pulley_teeth_count = 1;

    float slope = 1.0f / calculator.Get_Formular_Slope_steps_per_mm();
    __actuator_alpha.Init_FomularSlope(slope);
    __actuator_alpha.LinkStepper(board->GetStepper_Alpha(), slope);

    __actuator_beta.Init_FomularSlope(slope);
    __actuator_beta.LinkStepper(board->GetStepper_Beta(), slope);

    __actuator_gamma.LinkServo(board->GetServo_onVertical(), true);
    __actuator_gamma.Init_FomularSlope(slope);
}

void TeechWarehouse_Robot::_Init_ArmSolution(){
    __arm_config.arm_length = 205;
    // __arm_config.slave_gear_circle_length = 123.98;
    __arm_config.master_slope_steps_per_mm = 12.778;
    __arm_solution.LinkConfig(&__arm_config);
    this->_LinkArmSolution(&__arm_solution);
}

void TeechWarehouse_Robot::_InitStatic_PositionTriggers(){
    // PositionTrigger* trigger_X = HomeTrigger_Array::Instance().GetPositionTrigger(0);
    // trigger_X->AxisName = 'X';
    // trigger_X->SetTriggerPosition(1234);

    // PositionTrigger* trigger_a = HomeTrigger_Array::Instance().GetPositionTrigger(1);
    // trigger_a->AxisName = 'a';
    // trigger_a->SetTriggerPosition(DEG_TO_RAD * 90);
}

void TeechWarehouse_Robot::_InitStatic_Queues(){
    Queue_MoveBlock::Instance()._all_queue_ables = (Queue_able*)this->__all_move_blocks;
    // Init LineSegment queue head
    Queue_LineSegment::Instance()._all_queue_ables = (Queue_able*) this->__all_line_segments;
    LineSegment* line = Queue_LineSegment::Instance().GetRoom();
    line->TargetPosition.X = 0;
    line->TargetPosition.Y = 0;
    line->TargetPosition.Z = 0;
    line->TargetPosition.Roll = 0;
    line->TargetPosition.Pitch = 0;
    line->TargetPosition.Yaw = 0;
    line->PrintOUt();
    Queue_LineSegment::Instance().Deposit();
    Logger::Print("TeechWarehouse_Robot::Init", 83);
}