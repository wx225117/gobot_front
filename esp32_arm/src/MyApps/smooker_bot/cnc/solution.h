#pragma once
#include "CNC/solution/single_axis/cnc_single_axis.h"
class SmookerBot_CncSoution: public CncSingleAxis{
    public:
        // void Init();
        EnumAxis ConvertToEnum(char axis) override;



};