#pragma once

#include "MyBoards/board_base.h"
// #include "CNC/board/cnc_board_base.h"
#include "CNC/Actuator/stepper/actuator_fast_stepper.h"
#include "Robot/axis_homer/position_trigger_array.h" 
#include "VL6180X.h"
#include "FastAccelStepper.h"

#define HOME_TRIGGER_COUNT 92

class Twh2_Board: public BoardBase{
    public:
        void Init(bool is_on_reset) override;
        
        // We do nothing, just override the methods.
        void EnableMotor(EnumAxis_Inverseinematic axis_name, bool enable_it) {};

        FastAccelStepper* GetStepper_Alpha(){return __stepper_alpha;};
        FastAccelStepper* GetStepper_Beta(){return __stepper_beta;};

        void Test_PositionTriggers(int loops);
        void Test_VL6180x();

        void Test_SingleStepper(int index, int loops);
        void Test_DualStepper(int loops);


    protected:

    private:

        void __InitSteppers();
        PositionTrigger __all_position_triggers[HOME_TRIGGER_COUNT];
        // Servo __all_servos[SERVO_COUNT];
        FastAccelStepperEngine __stepper_engine = FastAccelStepperEngine();
        FastAccelStepper* __stepper_alpha = NULL;
        FastAccelStepper* __stepper_beta = NULL;
        FastAccelStepper* __stepper_rows[7];
        // HX711 __hx711;
        VL6180X sensor;
        


        

};