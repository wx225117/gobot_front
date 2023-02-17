// #include "circle_loop_arm_solution.h"
#include "sillicon_pump_arm_solution.h"
// #include "circle_loop_arm_solution_config.h"
#include "sillicon_pump_arm_solution_config.h"
// #include "Robot/axis_homer/position_trigger_array.h"

Sillicon_Pump_ArmSolution::Sillicon_Pump_ArmSolution(){
	Sillicon_Pump_Armsolution_Config config;
	__slope = config.Slope_Steps_per_box();
}
void Sillicon_Pump_ArmSolution::FK_to_IK(FKPosition_XYZRPW* from_fk,IKPosition_abgdekl* to_ik){
	FKPosition_XYZRPW* fk = from_fk;
	IKPosition_abgdekl* ik = to_ik;

	ik->Positions[AXIS_ALPHA] = fk->X * __slope ;
	Logger::Debug("Sillicon_Pump_ArmSolution::IK()");
	// Logger::Print("Sillicon_Pump_ArmSolution::FK_to_IK()  ")

	Logger::Print("IK output alpha", ik->Positions[AXIS_ALPHA]);
}

void Sillicon_Pump_ArmSolution::IK_to_FK(IKPosition_abgdekl* from_ik, FKPosition_XYZRPW*  to_fk){
	FKPosition_XYZRPW* fk = to_fk;
	IKPosition_abgdekl* ik = from_ik;
	
	fk->X = ik->Positions[AXIS_ALPHA] / __slope;
	// Logger::Debug("Sillicon_Pump_ArmSolution::FK()" );
	// Logger::Print("FK output X", fk->X);
}



