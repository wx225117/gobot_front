#include "gobot_house_hw.h"

#define PIN_ALPHA_ENABLE 18
#define PIN_BETA_ENABLE 16
#define PIN_MICRIO_STEP_2 21
#define PIN_MICRIO_STEP_1 22
#define PIN_MICRIO_STEP_0 23



#define MOTOR_MAX_SPEED 289
#define HOME_POSITION_ALPHA 0
#define HOME_POSITION_BETA 1

#define LINK_A 75
#define LINK_B 75

#define STEPS_PER_RAD 326   //2048 / 2*Pi
#define ACCELERATION_HOMIMG 2000
#define MAX_SPEED_HOMING 2000

// https://lastminuteengineers.com/28byj48-stepper-motor-arduino-tutorial/


GobotHouseHardware::GobotHouseHardware(){
}


FkPositionBase GobotHouseHardware::GetCurrentPosition(){
	return this->__current_fk_position; 
}


void GobotHouseHardware::HomeSingleAxis(char axis){
  Serial.print("[Debug] GobotHouseHardware::HomeSingleAxis() is entering\n" );
  Serial.print(axis);
  this->_homing_axis = axis;
  if (axis=='A'){
	this->objStepper_alpha.setAcceleration(ACCELERATION_HOMIMG);
	this->objStepper_alpha.setMaxSpeed(MAX_SPEED_HOMING);
	this->__homing_stepper = &this->objStepper_alpha;
	this->__homing_helper = &this->objHomeHelper_alpha;
  }else if (axis=='B'){
	this->objStepper_beta.setAcceleration(ACCELERATION_HOMIMG);
	this->objStepper_beta.setMaxSpeed(MAX_SPEED_HOMING);
	this->__homing_stepper = &this->objStepper_beta;
	this->__homing_helper = &this->objHomeHelper_beta;
  }
}



void GobotHouseHardware::_running_G28(){
	Serial.print("[Debug] GobotHouseHardware::running_G28() is entering \n");
	if (this->__homing_helper->IsTriged()){
		// End stop is trigered
		Serial.print(" Homed postion =    " );
		this->objStepControl.stop();
		this->__homing_stepper->setPosition(0);
		Serial.println(this->__homing_stepper->getPosition());
		// if (this->_homing_axis == 'A')
		// 	this->objActuator_Alpha.SetCurrentPos(HOME_POSITION_ALPHA);
		// 	// this->
		// else if (this->_homing_axis == 'B')
		// 	this->objActuator_Beta.SetCurrentPos(HOME_POSITION_BETA);
		this->State = IDLE;
	}else{
		Serial.print("Still homing\n");
		// Endstop is not trigered
		this->__homing_stepper->setTargetRel(-5000);
		this->objStepControl.moveAsync(*this->__homing_stepper);
	}
}




/*

	House                Y+
						 ^
			 r0          |
		  r1     d0      |
		r2      d1       |
	  r3       d3  d2    |               
	  -----------------(0,0)------Neck----------Head    --> X+
	  r4      d4  d5     |
	   r5       d6       |
		 r6     d7       |
		  r7             |

*/

// }
void GobotHouseHardware::IK(FkPositionBase* from_fk, IkPositionBase* to_ik){
	FkPosXY* fk = (FkPosXY*)(from_fk);
	IkPosAB* ik = (IkPosAB*)(to_ik);
	float rr1= fk->X * fk->X + fk->Y * fk->Y;

	float beta = acosf((LINK_A * LINK_A + LINK_B * LINK_B -  rr1 ) / (2* LINK_A * LINK_B));
	float r1 = sqrtf(rr1);
	float alpha_eef = acosf(fk->X/r1);
	float alpha_link = acosf((LINK_A * LINK_A + rr1 - LINK_B * LINK_B)/( 2*LINK_A * r1));
	float alpha = alpha_eef + alpha_link;
	ik->alpha = alpha * STEPS_PER_RAD;
	ik->beta =  beta * STEPS_PER_RAD; 
}

void GobotHouseHardware::FK(IkPositionBase* ik, FkPositionBase*  to_fk){

}



void GobotHouseHardware::init_gpio(){
	pinMode(PIN_ALPHA_ENABLE, OUTPUT);
	pinMode(PIN_BETA_ENABLE, OUTPUT);
	pinMode(PIN_MICRIO_STEP_0, OUTPUT);
	pinMode(PIN_MICRIO_STEP_1, OUTPUT);
	pinMode(PIN_MICRIO_STEP_2, OUTPUT);

	digitalWrite(PIN_ALPHA_ENABLE, LOW);
	digitalWrite(PIN_BETA_ENABLE, LOW);
	digitalWrite(PIN_MICRIO_STEP_0, LOW);
	digitalWrite(PIN_MICRIO_STEP_1, LOW);
	digitalWrite(PIN_MICRIO_STEP_2, LOW);

}
void GobotHouseHardware::Init_Linkage(){
	init_gpio();
	this->commuDevice = &this->objCommuUart; 

}

float GobotHouseHardware::GetDistanceToTarget(){
	IkPosAB current_ik;
	current_ik.alpha = (float)this->objStepper_alpha.getPosition();
	current_ik.alpha = (float)this->objStepper_beta.getPosition();
	FK(&current_ik, &this->__current_fk_position);
	
	float dx = this->__current_fk_position.X - this->__target_fk_position.X;
	float dy = this->__current_fk_position.Y - this->__target_fk_position.Y;
	float distance = sqrt(dx * dx + dy * dy);
	return distance;
}

void GobotHouseHardware::RunG1(Gcode* gcode) {
	//None blocking, move backgroundly.
	Serial.println("[Debug] GobotHouseHardware::RunG1()   \n");
	if (gcode->has_letter('F')){
		int speed = gcode->get_value('F');
		this->objStepper_alpha.setMaxSpeed(800);
	}
	float target_alpha = this->objStepper_alpha.getPosition();
	float target_beta = this->objStepper_beta.getPosition();
	Serial.print(target_alpha);
	Serial.print(",");
	Serial.print(target_beta);
	if (gcode->has_letter('A')) target_alpha = gcode->get_value('A');
	if (gcode->has_letter('B')) target_beta = gcode->get_value('B');
	Serial.print(" <-- from   alpha,beta   to --> ");
	Serial.print(target_alpha);
	Serial.print(",");
	Serial.println(target_beta);

	this->objStepper_alpha.setTargetAbs(target_alpha);
	this->objStepper_beta.setTargetAbs(target_beta);
	this->objStepControl.move(this->objStepper_alpha, this->objStepper_beta);
}
void GobotHouseHardware:: _running_G1(){
    if (this->GetDistanceToTarget() < 20){
      	this->State = IDLE;
    }
	Serial.print(".");
	delay(100);
}


