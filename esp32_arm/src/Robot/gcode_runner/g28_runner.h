#pragma once
#include "CNC/coordinate/cnc_axis.h"
#include "Robot/axis_homer/axis_homer.h"
#include "CNC/mover/mover_base.h"
#include "sync_gcode_runner_base.h"
#include "Robot/axis_homer/homer_diction.h"

class G28_Runner: public SyncGcodeRunnerBase{
    public:
        void Start() override ;
        bool IsDone() override;
        // void LinkMover(MoverBase* mover){this->__mover=mover;};
        // void LinkHomer(AxisHomer* homer){this->__homer=homer;};
        void LinkGcode(Gcode* gcode) override;
        float GetTriggerPosition(){return __last_homed_position;};

    protected:
        virtual void SetMoveBlock_ToHome(char axis_name, MoveBlock* mb);
        // virtual void SetMoveBlock_ToHome(EnumAxis_ForwardKinematic axis, MoveBlock* mb);
        // virtual AxisHomer* GetHomer(EnumAxis_ForwardKinematic axis);
        // virtual AxisHomer* GetHomer(EnumAxis_Inverseinematic axis);
        MoverBase* __mover;   //Only for stop()
        // AxisHomer* __homer;
        
    private:
        char __axis_name;
        // bool home_actuator_directly;
        float __last_homed_position;
};

