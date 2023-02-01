#include "touch_cell.h"
#include "MyLibs/basic/logger.h"

#define PPD2 0
#define PPD3 1
#define PPD4 2
#define PPD5 3
#define PPD6 4
#define PPD7 5
#define PPD8 6
#define PPD9 7
#define PPD10 8
#define PPD11 9

#define PPA0 10   // D14
#define PPA1 11   // D15
#define PPA2 12   // D16
#define PPA3 13   // D17


void TouchCell_2023::Init(uint8_t cell_id,  uint8_t installed_channel_count){
    
    for(int i=0; i<14; i++){
        if (i<installed_channel_count){
            __all_channels[i].Init(cell_id, i, TouchPad_Channel::EnumState::WROKING);
        }else{
            __all_channels->Init(cell_id, i, TouchPad_Channel::EnumState::NOT_INSTALLED);
        }
    }

}


bool TouchCell_2023::HasUpdate(){
    bool has_update = false;
    for(int i=0; i<2; i++){
        if (LastFlags[i] != CurrentFlags[i]){
            has_update = true;
        }
    }
    return has_update;
}

const char* TouchCell_2023::GetName(int point_id){
    String name = "u";
    name.concat(this->Address);
    name.concat("p");
    name.concat(point_id);
    name.toCharArray(this->__mqtt_topic_substring, 30);
    return this->__mqtt_topic_substring;
}


// bool TouchCell_2023::CompareCurrentAndLast(){

//     return _has_update;
// }

void TouchCell_2023::CopyCurrentToLast(){
    for(int i =0; i<4; i++){
        this->LastFlags[i] = this->CurrentFlags[i];
    }
}

// paras: bit_index=[0..15]
bool TouchCell_2023::IsBitUpdated(int bit_index){
    int byte_index = 0;
    if (bit_index>=8){
        byte_index++;
        bit_index -=8;
    }
    int bit_mask = (CurrentFlags[byte_index] ^ LastFlags[byte_index]) & (1<<bit_index);
    if (bit_mask > 0){
        // Logger::Debug("TouchCell_2023::IsBitUpdated()");
        // Logger::Print("Cell address", Address);
        // Logger::Print("Current Flag 0", CurrentFlags[0]);
        // Logger::Print("Last Flag 0", LastFlags[0]);
        // Logger::Print("Current Flag 1", CurrentFlags[1]);
        // Logger::Print("Last Flag 1", LastFlags[1]);
        // Logger::Print("bit_mask", bit_mask);
        // Logger::Print("Updated Byte index", byte_index);
        // Logger::Print("Updated Bit_index", bit_index);
        return true;
    }
    return false;
}
const char* TouchCell_2023::GetMqttPayload(int bit_index){
    int byte_index = 0;
    if (bit_index >= 8){
        byte_index++;
        bit_index -= 8;
    }
    if (CurrentFlags[byte_index] & (1<<bit_index))
        return "ON";
    return "OFF";
}


