#include "text_message_line.h"

CommandText::Enum_Category CommandText::GetCategory(){
    if (this->IsPrefix("lua:")){
        __category = LUA;
    }else if (this->IsPrefix("gcode:")){
        __category = GCODE;
    }else if (this->IsPrefix("app:")){
        __category = APP;
    }else{
        __category = __preset_catetory;
    }
}
