#include "solution.h"

void Vsc_CncSoution::Init(Vsc_Board* board ){
    this->_AXIS = AXIS_ALPHA;

    this->__mover.Init(board);
    this->_mover_base = &this->__mover;
    
    this->_LinkEef(board->GetEef());
    this->_config->LinkHomers(board->GetCncHomers());

    //So all pid controllers are configable via mcode. example: 'M130 N0 P1 I2 D3'
    this->_LinkPidControllers(board->GetPidControllers());

    board->EnableMotor(AXIS_ALPHA, false);
	this->_cnc_board = board;

    HomingConfig* config = this->_config->GetAxisHomers()->GetAxisHomer(AXIS_ALPHA)->GetHomingConfig();
    config->IsDirectionToMax = true;
    config->Speed = TWO_PI / 60;  // 60 seconds Per loop.
    config->Accelleration = 0.1;
    config->DistanceToGo = TWO_PI;

}

EnumAxis Vsc_CncSoution::ConvertToEnum(char axis){
    switch (axis){
        case 'A':
            return AXIS_ALPHA;
            break;

        default:
            Serial.print("[Error] GobotMainCnc::ConvertToEnum() Unknown axis= ");
            Serial.println(axis);
            while(true){
                Serial.print("E ");
                delay(500);
            }
            return AXIS_ALPHA;
            break;
    }
}
