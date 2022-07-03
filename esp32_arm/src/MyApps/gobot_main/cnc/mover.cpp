#include "mover.h"
#include "actuator_alpha_beta.h"

void Gobot_CncMover::Init(BoardBase* board, StepControl* stepControl){
    GobotMain_ActuatorAlphaBetaMechanic config;
    Board_GobotMain* myboard = (Board_GobotMain*) board;
    this->__alpha.LinkStepper(myboard->GetStepper(AXIS_ALPHA), config.GetStepsPerCncUnit());
    this->LinkStepper_asAlpha(&this->__alpha);

    this->__beta.LinkStepper(myboard->GetStepper(AXIS_BETA), config.GetStepsPerCncUnit());
    this->LinkStepper_asBeta(&this->__beta);
    this->LinkStepControl(stepControl);
}
