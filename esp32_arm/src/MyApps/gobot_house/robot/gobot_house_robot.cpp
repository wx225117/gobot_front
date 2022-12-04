#include "gobot_house_robot.h"
#include "Robot/mcode_runner/mcode_os.h"

void GobotHouseRobot::Init(GobotMain_Board* board){
    Logger::Debug("Vsc_ArmSoution::Init()");

    this->_g28_runner=&this->g28_runner;
    g28_runner.Init(&mover, &arm_solution);
    this->_LinkMover(&mover);
    
    Queue_MoveBlock::Instance()._all_queue_ables = (Queue_able*)this->__all_move_blocks;
}