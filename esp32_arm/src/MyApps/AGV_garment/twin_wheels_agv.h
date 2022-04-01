/*
.   Objects:
.       BoxMover
.       TwinWheels
.
.   Sensors:
.       I2C-A:  APDS9960  left
.       i2C-B:  APDS9960 right
.       GPIO:   HS04
*/


/*    
.     
.      <-----------------------------------------------------------------------------\
,     |                               ^                                              | 
,    F_Paused          S_Paused       |                P_Paused                      |
.     |  ^             |     ^        |[Fast mark]      |    ^                       | [Command Move]                      ^                             
.     |  |             |     |        |                 |    |                       |                           
.   Fast Moving  --> Slow_Moving ------------------>   Parking  ---------> Parked ---^  
.    [slow mark]                       [Park command]   (Super slow)[Sleep command] 
.   from track sensor                                                    
.                                                                        
*/


#pragma once
#include <HCSR04.h> 



#include "track_sensor_dual_9960.h"
#include "SimpleFOC.h"

class TwinWheelsAgv{
    public:
        enum AGV_STATE{
            FAST_MOVING = 1,
            FAST_MOVING_PAUSED = 2,
            SLOW_MOVING = 3,
            SLOW_MOVING_PAUSED = 4,
            PARKING = 5,
            PARKING_PAUSED = 6,
            PARKED = 7,
        };

        TwinWheelsAgv();
        void Init();
        void SpinOnce();
        TwinWheelsAgv::AGV_STATE GetState(){return this->_State;};
        void ToState(TwinWheelsAgv::AGV_STATE state);

    protected:
        AGV_STATE _State;
        void Forwarding();

    private:
        float __fast_velocity = 100;
        float __slow_velocity = 60;
        float __parking_velocity = 10;
        float common_speed;
        float diff_speed;  //left faster is positive.

        TrackSensor_Dual9960* trackSensor;
        UltraSonicDistanceSensor *obstacleSensor; // = UltraSonicDistanceSensor(HS04_PIN_ECHO,HS04_PIN_TRIG); //initialisation class HCSR04 (trig pin , echo pin)
        HardwareSerial * leftWheel_commu;
        HardwareSerial * rightWheel_commu;

        bool DoParking();

};