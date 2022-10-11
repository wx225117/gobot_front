#pragma once

#include "MyLibs/list/pid_controllers_list.h" //todo:  remove

#include "CNC/board/cnc_board_base.h"   // todo:  remove
#include "CNC/gcode/gcode_consumer.h"  //todo:  remove
#include "kinematic_config.h" 
#include "../coordinate/coordinate_base.h"
#include "../coordinate/cnc_axis.h"
#include "../mover/cnc_mover_base.h"
#include "Robot/axis_homer/home_trigger_array.h"  //todo:  remove
#include "Robot/eef/eef_standard_code.h"  //todo:  remove

// enum class MoverState{    //TODO: rename to MoverState?
//     IDLE,       // Motor is enabled.
//     SLEEPING,   // Motor is disabled
//     RUNNING_G1,
// };

class ArmSolutionBase{
    public:

        virtual bool GetCurrentPosition(FkPositionBase* position_fk);

        // Todo:  should remove these two lines?
        virtual float GetDistanceToTarget_FK();
        virtual float GetDistanceToTarget_IK();


        virtual void _SetCurrentPositionAsHome(EnumAxis_ForwardKinematic homing_axis);
        void ForceStopMover();  //TODO:  para is MoverAction:  EnableActuator, ForceStop, DisableActuator. 

        virtual bool _CutGcodeLine_ToSegmentQueue(Gcode* gcode);
        virtual void __ConvertSegment_ToMoveBlockQueue(LineSegment* line);
        
    protected:
        virtual void IK(FkPositionBase* from_fk, IkPositionBase* to_ik);
        virtual void FK(IkPositionBase* from_ik,FkPositionBase* to_fk);

        KinematicConfig _config_base;    //TODO:  rename to _kinamatic_config

        bool is_absolute_position = true;

    private:

};

