#pragma once

// #include "Axis/SingleAxisBase.hpp"
#include <Robot/Gcode.h>
#include "Robot/Commu/CommuDeviceBase.h"

// #include "Axis/SingleAxis.h"
// #include "Robot/Actuator/ActuatorControllerBase.h"
// #include "Robot/Actuator/ActuatorBase.h"
// struct ik_position{
//     int alpha;
//     int beta;
// };



class FkPositionBase{

};
class FkPosX: public FkPositionBase{
    public:
        float x;
};
class FkPosXY: public FkPositionBase{
    public:
        float x;
        float y;
};
class FkPosXYZ: public FkPositionBase{
    public:
        float x;
        float y;
        float z;
};

class fkPosXYZA: public FkPositionBase{
    public:
        float x;
        float y;
        float z;
        float a;
};

class fkPosXYZAB: public FkPositionBase{
    public:
        float x;
        float y;
        float z;
        float a;
        float b;
};
class fkPosXYZABC: public FkPositionBase{
    public:
        float x;
        float y;
        float z;
        float a;
        float b;
        float c;
};

class fkPosXYUVW: public FkPositionBase{
    public:
        float x;
        float y;
        float z;
        float u;
        float v;
        float w;
};

class IkPositionBase{

};
class IkPosX: public IkPositionBase{
    public:
        float alpha;
};
class IkPosXY: public IkPositionBase{
    public:
        float alpha;
        float beta;
};


/**
 * RobotBase has NO axis! 
 *      Reasons:
 *          1. Doesn't know how may axis. 
 *      ?? Or has at least one axis? 
 *      ?? Saying has no actuator, driver, sensor ?
 * RobotBase has NO ActuatorController!
 *          The ActuatorController might be inside of ActuatorDriver.
 *                                 might be no ActuatorCotroller in the whole system.
*/
class RobotBase{
    public:
        enum STATE{
            IDLE,
            RUNNING_G1,
            RUNNING_G4,
            RUNNING_G28
        };
        STATE State = IDLE;
        void RunGcode(Gcode* gcode);
        void SpinOnce();
        virtual void Init_Linkage();
        virtual void HomeAllAxises();   //??
        
        // bool IsIdle(){return this->robot_is_idle;};
        // virtual void Init();

    protected:
        RobotBase(){};
        void LinkCommuDevice(CommuDeviceBase* commuDevice){this->commuDevice=commuDevice;};
        virtual void MoveToTargetPosition();
        virtual void SpinOnce_BaseEnter();
        virtual void SpinOnce_BaseExit();
        virtual IkPositionBase* IK(FkPositionBase* fk);
        virtual FkPositionBase* FK(IkPositionBase* ik);
        virtual void RunG1(Gcode* gcode);   //None blocking, move backgroundly.
        virtual void RunG6(Gcode* gcode);   //Block mode
        void RunG4(Gcode* gcode);
        virtual std::string GetHomeTrigerStateString();
        CommuDeviceBase* commuDevice;
        // bool robot_is_idle = true;    //TODO: Will be instead of commu_buffer_length > 0.
        bool is_absolute_position = true;
        // Just for fun, don't remove below comment !!
        // void OnFinishedGcode2(void(*callback)()) {__output_message2 = callback;};
        // void OnFinishedGcode3(void(*callback)()) {__output_message2 = callback;};
        // void OnFinishedGcode4(void(*callback())) {__output_message3 = callback;};
        // void OnFinishedGcode5(void(*callback)()) {__output_message3 = callback;};
        // void * __output_message2;
        // void (* __output_message3);
        virtual void _running_G1();
        virtual void _running_G28();
    
    private:
        int test_int;
        void _base_spin_once();
        void __running_G4();
        long __g4_start_timestamp;
        int __g4_time_second;



};

