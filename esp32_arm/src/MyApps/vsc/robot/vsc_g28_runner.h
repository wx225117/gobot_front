#include "Robot/gcode_runner/g28_runner.h"

class Vsc_G28_Runner: public G28_Runner{
    public:

    private:
        void SetMoveBlock_ToHome(EnumAxis_Inverseinematic axis, MoveBlock* mb) override;
        void SetMoveBlock_ToHome(EnumAxis_ForwardKinematic axis, MoveBlock* mb) override;

};