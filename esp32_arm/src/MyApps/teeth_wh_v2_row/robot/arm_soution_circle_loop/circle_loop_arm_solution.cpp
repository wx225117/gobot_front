#pragma once

#include "circle_loop_arm_solution.h"
#include "circle_loop_arm_solution_config.h"
#include "Robot/axis_homer/position_trigger_array.h"

Twh2_Circleloop_ArmSolution::Twh2_Circleloop_ArmSolution(){
	Twh2_Circleloop_Armsolution_Config config;
	__slope = config.Slope_Steps_per_column();
}
void Twh2_Circleloop_ArmSolution::FK_to_IK(FKPosition_XYZRPW* from_fk,IKPosition_abgdekl* to_ik){
	FKPosition_XYZRPW* fk = from_fk;
	IKPosition_abgdekl* ik = to_ik;

	ik->Positions[AXIS_ALPHA] = fk->Roll / __slope - __offset ;
	Logger::Debug("Twh2_Circleloop_ArmSolution::IK()");
	// Logger::Print("IK output alpha", ik->alpha);
	Logger::Print("IK output alpha", ik->Positions[AXIS_ALPHA]);
}

void Twh2_Circleloop_ArmSolution::IK_to_FK(IKPosition_abgdekl* from_ik, FKPosition_XYZRPW*  to_fk){
	FKPosition_XYZRPW* fk = to_fk;
	IKPosition_abgdekl* ik = from_ik;
	
	fk->Roll = __slope * (ik->Positions[AXIS_ALPHA] + __offset);
	Logger::Debug("Twh2_Circleloop_ArmSolution::FK()" );
	Logger::Print("FK output A", fk->Roll);
}


