#pragma once

#include "line_segment.h"
#include "MyLibs/basic/queue/queue_base.h"

#define LINE_SEGMENTS_QUEUE_SIZE 8


class gs_LineSegment_Queue: public QueueBase<LineSegment>{
    public:
        static gs_LineSegment_Queue& Instance(){
            static gs_LineSegment_Queue instance;
            return instance;
        };
        LineSegment* Withdraw() {return this->_Withdraw();};
        LineSegment* GetRoom() {return this->_GetRoom();};
        LineSegment* GetHeadLineSegment() {return this->_GetHeadObject();};
        void DeepCopyCurrentFkPositionTo(FKPosition_XYZRPW* position);

    private:
        gs_LineSegment_Queue(){this->_Init("Line_Segments", LINE_SEGMENTS_QUEUE_SIZE, sizeof(LineSegment));};
        

};