#include "line_segment.h"


void LineSegment::Calculate_distance_time(FKPosition_XYZRPY* start_position){
    this->vector_distance = 123;
    this->required_time = vector_distance / Speed;
}

void LineSegment::DeepCopyTo(LineSegment* the_copy){
    the_copy->TargetPosition->X = this->TargetPosition->X;
    the_copy->TargetPosition->Y = this->TargetPosition->Y;
    the_copy->TargetPosition->Z = this->TargetPosition->Z;
    the_copy->TargetPosition->Roll = this->TargetPosition->Roll;
    the_copy->TargetPosition->Pitch = this->TargetPosition->Pitch;
    the_copy->TargetPosition->Yaw = this->TargetPosition->Yaw;
}
