#include "cnc_machine.h"



void VscMachine::Init(char solution){
        // this->max_acceleration_alpha = 200;
        // this->max_speed_alpha = 200;

        // this->Homing_speed_alpha = 100;
        // this->Homing_acceleration_alpha = 200;
        this->_HOMING_DIR_IS_TO_MAX[AXIS_ALPHA] = 1;
        this->_HOMING_SPEED[AXIS_ALPHA] = 100.0f;


}

void VscMachine::PrintOut(){
    Serial.print("\n\n===============================================");
    Serial.print("\nSpringMakerMachine::PrintOut()");
    Serial.print("\n     Home_position (a) = (");
    Serial.print(this->HomedPosition(AXIS_ALPHA));
    Serial.print(" , ");

    Serial.print("\n     Homing_speed_alpha = ");
    // Serial.print(this->Homing_speed_alpha);
    Serial.print(this->HomingSpeed(AXIS_ALPHA));
    // Serial.print("\n     Homing_acceleration_alpha = ");
    // Serial.print(this->Homing_acceleration_alpha);

    // Serial.print("\n\n   max_speed_alpha = ");
    // Serial.print(max_acceleration_alpha);


    Serial.print("\n----------------------------------------------\n\n");
}

