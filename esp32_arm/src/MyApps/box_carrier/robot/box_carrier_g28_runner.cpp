#include "box_carrier_g28_runner.h"


void BoxCarrier_G28_Runner::Init(Board2204Cnc* board, MoverBase* mover){
    Logger::Info("BoxCarrier_G28_Runner::Init() Hoiming_config");
    this->__mover = mover;

    homing.IsDirectionToMax = true;
    homing.Speed = TWO_PI / 60;  // 60 seconds Per loop.
    homing.Accelleration = 0.1;
    homing.DistanceToGo = TWO_PI;
    
    Logger::Info("BoxCarrier_G28_Runner::Init() Alpha axis home_triggers");
    PositionTrigger* trigger;
    trigger = board->GetPositionTrigger(0);
    trigger->SetTriggerPosition(TWO_PI* 1 / 386);      // @01 pitch, total 386 pitches,    
    __homer.AppendPositionTrigger(trigger);

    trigger = board->GetPositionTrigger(1);
    trigger->SetTriggerPosition(TWO_PI * 90 / 386);     //at pitch 90 , total 386 pitches, value = TWOPI *(90/386)
    __homer.AppendPositionTrigger(trigger);
}


void BoxCarrier_G28_Runner::SetMoveBlock_ToHome(EnumAxis_Inverseinematic axis, MoveBlock* mb){
    Logger::Error("BoxCarrier_G28_Runner::SetMoveBlock_ToHome() ---- Inverse axis");
    Logger::Print("axis", axis);
    // MoveBlock_SingleActuator* aa;
    // switch (axis){
    //     case AXIS_ALPHA:
    //         Logger::Print("BoxCarrier_G28_Runner::SetMoveBlock_ToHome()  point", 21);
    //         mb->DeepReset_ToDefault();
    //         Logger::Print("BoxCarrier_G28_Runner::SetMoveBlock_ToHome()  point", 22);
    //         aa = &mb->MoveBlocks[0];
    //         Logger::Print("BoxCarrier_G28_Runner::SetMoveBlock_ToHome()  point", 23);
    //         aa->IsAbsTargetPosition = false;
    //         aa->TargetPosition = 200;
    //         aa->Speed = 555;
    //         aa->Acceleration = 33;
    //         break;
    //     default:
    //         Logger::Error(" BoxCarrier_G28_Runner::SetMoveBlock_ToHome() Unknown axis");
            
    //         break;
    // }
    // Logger::Print("BoxCarrier_G28_Runner::SetMoveBlock_ToHome()  point", 99);
}

void BoxCarrier_G28_Runner::SetMoveBlock_ToHome(EnumAxis_ForwardKinematic axis, MoveBlock* mb){
    Logger::Debug("BoxCarrier_G28_Runner::SetMoveBlock_ToHome()   ---- Forward axis" );
    Serial.print(char(axis));
    Logger::Print("\taxis", char(axis));
    MoveBlock_SingleActuator* move;
    switch (axis){
        case AXIS_X:
            Logger::Print("BoxCarrier_G28_Runner::SetMoveBlock_ToHome()  point", 21);
            mb->DeepReset_ToDefault();
            Logger::Print("BoxCarrier_G28_Runner::SetMoveBlock_ToHome()  point", 22);
            move = &mb->MoveBlocks[AXIS_ALPHA];
            Logger::Print("BoxCarrier_G28_Runner::SetMoveBlock_ToHome()  point", 23);
            move->IsAbsTargetPosition = false;
            move->TargetPosition = homing.DistanceToGo;
            move->Speed = homing.Speed;
            move->Acceleration = homing.Accelleration;
            break;
        default:
            Logger::Error(" BoxCarrier_G28_Runner::SetMoveBlock_ToHome() Unknown axis");
            
            break;
    }
    Logger::Print("BoxCarrier_G28_Runner::SetMoveBlock_ToHome()  point", 99);
}

AxisHomer* BoxCarrier_G28_Runner::GetHomer(EnumAxis_ForwardKinematic axis){
    return &__homer;
}

AxisHomer* BoxCarrier_G28_Runner::GetHomer(EnumAxis_Inverseinematic axis){
    return &__homer;
}
