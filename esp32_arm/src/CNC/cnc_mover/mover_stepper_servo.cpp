#include "mover_stepper_servo.h"


void CncMover_StepperServo::LinkStepper_asAlpha(Stepper* stepper){
    this->__stepper_alpha = stepper;
}

void CncMover_StepperServo::LinkServo_asBeta(Servo* servo){
    this->__servo_beta = servo;
}

void CncMover_StepperServo::AllMotorsMoveTo(bool is_absolute_position, float* motor_position, int motors_count){
    if (motors_count==2){
        if (is_absolute_position){
            this->__stepper_alpha->setTargetAbs(motor_position[0]);
        } else {
            this->__stepper_alpha->setTargetRel(motor_position[0]);
        }
        this->__stepControl.moveAsync(*this->__stepper_alpha);
        this->__servo_beta->write(motor_position[1]);
    }
}

void CncMover_StepperServo::AllMotorStop(){
    this->__stepControl.stop();
}

void CncMover_StepperServo::SingleMotorMoveTo(bool is_absolute_position, char motor_name, float motor_position){
    if (motor_name == 'A'){
        if (is_absolute_position){
            this->__stepper_alpha->setTargetAbs(motor_position);
        }else{
            this->__stepper_alpha->setTargetRel(motor_position);
        }
        this->__stepControl.moveAsync(*this->__stepper_alpha);
    }else if (motor_name == 'B'){
        if (is_absolute_position){
            this->__servo_beta->write(motor_position);
        }else{
            log_w("CncMover_StepperServo::SingleMotorMoveTo()  "," Not Implemented");
        }
    }else{
        log_w("CncMover_StepperServo::SingleMotorMoveTo() axisname= ", motor_name );
    }
}

float CncMover_StepperServo::GetMotorPosition(char motor_name){
    if (motor_name == 'A'){
        return this->__stepper_alpha->getPosition();
    }else if (motor_name == 'B'){
        return this->__servo_beta->read();
    }else{
        log_w("CncMover_StepperServo::SingleMotorMoveTo() axisname= ", motor_name );
    }
}

void CncMover_StepperServo::SetMotorPosition(char motor_name, float as_current_position){
    if (motor_name == 'A'){
        this->__stepper_alpha->setPosition(as_current_position);
    }else if (motor_name == 'B'){
        log_w("CncMover_StepperServo::SingleMotorMoveTo()  "," Not Implemented");
    }else{
        log_w("CncMover_StepperServo::SingleMotorMoveTo() Unkonwn axisname= ", motor_name );
    }
}
float CncMover_StepperServo::GetDistanceToTarget(){
    return this->__stepper_alpha->getDistanceToTarget();
}
