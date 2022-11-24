#include "move_block.h"



// void MoveBlock::DeepCopyTo(Queue_able* copy){
void MoveBlock::DeepCopyTo(MoveBlock* copy){
    MoveBlock* cp = (MoveBlock*) copy;
    // TODO: any risk of mem_copy ?
    for(int a=0; a<CNC_AXIS_COUNT; a++){
        // cp->MoveBlocks[a].axis = this->MoveBlocks[a].axis;
        cp->MoveBlocks[a].Acceleration = this->MoveBlocks[a].Acceleration;
        cp->MoveBlocks[a].Speed = this->MoveBlocks[a].Speed;
        // cp->MoveBlocks[a].IsAbsTargetPosition = this->MoveBlocks[a].IsAbsTargetPosition;
        cp->MoveBlocks[a].TargetPosition = this->MoveBlocks[a].TargetPosition;
    }
}

void MoveBlock::DeepCopyToIkPosition(IKPosition_abgdekl* to_position){
    to_position->alpha =  this->MoveBlocks[AXIS_ALPHA].TargetPosition;
    to_position->beta =  this->MoveBlocks[AXIS_BETA].TargetPosition;
    to_position->gamma =  this->MoveBlocks[AXIS_GAMMA].TargetPosition;
    to_position->delta =  this->MoveBlocks[AXIS_DELTA].TargetPosition;
    to_position->eppsilon =  this->MoveBlocks[AXIS_EPSILON].TargetPosition;
    to_position->kappa=  this->MoveBlocks[AXIS_KAPPPA].TargetPosition;
    to_position->lambda =  this->MoveBlocks[AXIS_LAMBDA].TargetPosition;
}


void MoveBlock::DeepReset_ToDefault(){
    // Logger::Debug("MoveBlock::DeepReset_ToDefault()");
    for(int a=0; a<CNC_AXIS_COUNT; a++){
        // Logger::Print("for  i", i );
        this->MoveBlocks[a].IsAbsTargetPosition = false;
        this->MoveBlocks[a].TargetPosition = 0;
        this->MoveBlocks[a].Speed = 0;
    }
}

