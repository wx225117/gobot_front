#pragma once


#include "CNC/cnc_machine_base.h"
// #include "CNC/cnc_config_base.h"

class CncScaraMachine: public CncMachineBase{
    public:
        virtual float GetHomingSpeed(char axis_name);
        // value is vary for each gobot-house robot.
        float Homed_position_alpha_in_degree;
        // value is vary for each gobot-house robot.
        float Homed_position_beta_in_degree;
        float motor_steps_per_round;

        //The length of Master linkage, unit is mm
        float LINK_A = 75.0;
        //The length of slave linkage, unit is mm
        float LINK_B = 75.0;

        // when microstep==1 Should be 736.25 , validated.
        float STEPS_PER_RAD_ALPHA;  
        float MAX_ACCELERATION_ALPHPA; // = 600 * this->MOTOR_MICRO_STEPS;
        float MAX_STEPS_PER_SECOND_ALPHA; // = 1200 * this->MOTOR_MICRO_STEPS;    // tested 1200



        int Homing_acceleration_alpha; 


        
    protected:
        int Homing_speed_alpha;    
        int Homing_acceleration_beta;
        int8_t HOMING_DIR_ALPHA = 1;
        int8_t HOMING_DIR_BETA = -1;
        // float Homing_speed_beta;

};