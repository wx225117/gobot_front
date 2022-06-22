#include "mover_stepper_servo.h"


void CncMover_StepperServo::LinkStepper_asAlpha(ActuatorStepper* alpha){
    alpha->SetCurrentPositionAs(0);
    alpha->GetLinkedStepper()->setPosition(0);
    this->__actuator_alpha = alpha;
}

void CncMover_StepperServo::LinkServo_asBeta(ActuatorServo* servo){
    this->__actuator_beta = servo;
}
#include "MyLibs/calculator.h"

void CncMover_StepperServo::SetSpeed(float speed){
    if (this->__moving_actuator_flags == 0x01){
        //speed is for alpha
        this->__actuator_alpha->SetSpeed(speed);
    }else if (this->__moving_actuator_flags == 0x02){
        //spped is for beta
        this->__actuator_beta->SetSpeed(speed);
    }else if (this->__moving_actuator_flags == 0x03){
        //speed is for both.   TODO:  speed = alpha * alpha + beta * beta;
        this->__actuator_alpha->SetSpeed(speed);
        this->__actuator_beta->SetSpeed(speed);
    }else{
        Serial.print("[Warn] CncMover_StepperServo::SetSpeed() Unknown flag = ");
        Serial.println(this->__moving_actuator_flags);
    }
}

// alpha = flags.bits[0]
// beta = flags.bits[1]
void CncMover_StepperServo::AllActuatorsMoveTo(uint8_t is_absolute_position_flags, float* positions_in_cnc_unit){
    bool is_absolute_position;
    Stepper* alpha = this->__actuator_alpha->GetLinkedStepper();
    uint8_t target_motor_flags = this->__moving_actuator_flags;

    // Step1:  Set target motor position. determin absolute or relative.
    if (target_motor_flags == 0x01){
        // set alpha position
        is_absolute_position = (is_absolute_position_flags & 0x01) > 0;
        this->__actuator_alpha->SetTargetPositionTo(is_absolute_position, positions_in_cnc_unit[0]);

    }else if (target_motor_flags == 0x02){
        // set beta position
        is_absolute_position = (is_absolute_position_flags & 0x02) > 0;
        this->__actuator_beta->SetTargetPositionTo(is_absolute_position, positions_in_cnc_unit[1]);

    }else if (target_motor_flags == 0x03){
        // set alpha position
        is_absolute_position = (is_absolute_position_flags & 0x01) > 0;
        this->__actuator_alpha->SetTargetPositionTo(is_absolute_position, positions_in_cnc_unit[0]);
        // and set beta position
        is_absolute_position = (is_absolute_position_flags & 0x02) > 0;
        this->__actuator_beta->SetTargetPositionTo(is_absolute_position, positions_in_cnc_unit[1]);
    }

    //Step2:  move one or all motors.
    if (target_motor_flags == 0x01){
        // Move alpha only
        this->__stepControl.moveAsync(*alpha);

    }else if (target_motor_flags == 0x02){
        // move beta only
        this->__actuator_beta->StartToMove();

    }else if (target_motor_flags == 0x03){
        // Both motors will move. Should end at the same time. so, one of the motor should be slower.
        float alpha_time_in_second = this->__actuator_alpha->GetNeededSeconds();
        float beta_time_in_second = this->__actuator_beta->GetNeededSeconds();
        if (alpha_time_in_second > beta_time_in_second){
            // beta should be slower, saying renew its speed.
            this->__actuator_beta->RenewSpeed(alpha_time_in_second);
        }else{
            // alpha should be slower, saying renew its speed.
            this->__actuator_alpha->RenewSpeed(beta_time_in_second);
        }
        this->__stepControl.moveAsync(*alpha);
        this->__actuator_beta->StartToMove();

    }else{
        Serial.print("[Error] CncMover_StepperServo::AllMotorsMoveTo()  target_motor_flags= ");
        Serial.println(target_motor_flags);
    }
    this->__moving_actuator_flags = target_motor_flags;
}

void CncMover_StepperServo::AllActuatorsStop(){
    this->__stepControl.stop();
    this->__actuator_beta->Stop();
}

void CncMover_StepperServo::SingleActuatorStop(char actuator_name){
    if (actuator_name == 'A'){
        this->__actuator_alpha->Stop();
        this->__moving_actuator_flags -= 0x01;

    }else if (actuator_name=='B'){
        this->__actuator_beta->Stop();
        this->__moving_actuator_flags -= 0x02;
    }else{
        Serial.print("[Error] CncMover_StepperServo::SingleMotorStop() Unknown actuator_name= ");
        Serial.println(actuator_name);
    }
}


void CncMover_StepperServo::SingleActuatorMoveTo(char actuator_name, bool is_absolute_position, float position_in_cnc_unit){
    if (actuator_name == 'A'){
        this->__moving_actuator_flags = 0x01;
        this->__actuator_alpha->SetTargetPositionTo(is_absolute_position, position_in_cnc_unit);
        Stepper* stepper = this->__actuator_alpha->GetLinkedStepper();
        this->__stepControl.moveAsync(*stepper);

    }else if (actuator_name == 'B'){
        this->__moving_actuator_flags = 0x02;
        this->__actuator_beta->SetTargetPositionTo(is_absolute_position, position_in_cnc_unit);
        this->__actuator_beta->StartToMove();

    }else{
        log_w("CncMover_StepperServo::SingleMotorMoveTo() axisname= ", actuator_name );
    }
}

float CncMover_StepperServo::GetSingleActuatorCurrentPosition_InCncUnit(char actuator_name){
    if (actuator_name == 'A'){
        return this->__actuator_alpha->GetCurrentPosition_InCncUnit();

    }else if (actuator_name == 'B'){
        return this->__actuator_beta->GetCurrentPosition_InCncUnit();

    }else{
        log_w("CncMover_StepperServo::SingleMotorMoveTo() axisname= ", actuator_name );
    }
    return 0;
}

void CncMover_StepperServo::SetActuatorCurrentCncPositionAs(char actuator_name, float as_current_position){
    if (actuator_name == 'A'){
        this->__actuator_alpha->SetCurrentPositionAs(as_current_position);
    }else if (actuator_name == 'B'){
        this->__actuator_beta->SetCurrentPositionAs(as_current_position);
    }else{
        log_w("[Warn] CncMover_StepperServo::SingleMotorMoveTo() Unkonwn axisname= ", actuator_name );
    }
}
float CncMover_StepperServo::GetDistanceToTarget_InCncUnit(){
    float alpha_distance = 0;
    if((this->__moving_actuator_flags & 0x01) > 0){
       alpha_distance = this->__actuator_alpha->GetDistanceToTarget_InCncUnit();
    }
    float beta_distance = 0;
    if ((this->__moving_actuator_flags & 0x02) > 0){
       beta_distance = this->__actuator_beta->GetDistanceToTarget_InCncUnit();
    }
    bool debug= false;
    if(debug){
        Serial.print("[Debug] CncMover_StepperServo::GetDistanceToTarget_InCncUnit() alpha = ");
        Serial.print(alpha_distance);
        Serial.print("  beta = ");
        Serial.println(beta_distance);
    }
    return sqrt(alpha_distance * alpha_distance + beta_distance * beta_distance);

}

void CncMover_StepperServo::SetActuatorSpeed(char actuator_name, float speed_per_second){
    if (actuator_name =='A'){
        this->__actuator_alpha->SetSpeed(speed_per_second);
    }else if(actuator_name == 'B'){
        this->__actuator_beta->SetSpeed(speed_per_second);
    }else{
        Serial.println("[Error]() CncMover_StepperServo::SetActuatorSpeed");
    }
}

bool CncMover_StepperServo::ActuatorIsMoving(char actuator_name) {
    if (actuator_name=='A'){
        return this->__stepControl.isRunning();
    }else if (actuator_name=='B'){
        return this->__actuator_beta->IsMoving();
    }else{
        Serial.print("[Error] CncMover_StepperServo::MotorIsMoving() Unknown motor_name= ");
        Serial.println(actuator_name); 
    }
    return false;
}
