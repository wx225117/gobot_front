#include "remote_var_chars.h"
#include "MyLibs/utility/logger.h"
#include "SPIFFS.h"


void RemoteVar_Chars::InitFilename(const char* filename){
    __write_to_file = true;
    // memccpy(__fs_filename, filename,20 );
    __fs_filename.CopyFrom(filename);
}
void RemoteVar_Chars::onGot_MqttMessage(const char* payload, uint16_t payload_len){
    if (__write_to_file){
        File file = SPIFFS.open(__fs_filename.GetChars(), FILE_WRITE);
        if (!file) {
            Serial.println("There was an error opening the file for writing");
            return;
        }
        // if (file.print(payload,)) {
        //     Serial.println("File was written");
        // } else {
        //     Serial.println("File write failed");
        // }
        file.close();
        return;
    }

    for (int i=0; i< payload_len; i++){
        __remote_value[i] = *(payload +i);
        if (i >= REMOTE_VAR_CHARS_MAX_BYTES_OF_BUFFER){
            Logger::Error("RemoteVar_Chars::onGot_MqttMessage");
            Logger::Print("reason: received payload is oversize ", payload_len );
            Logger::Halt("I can sing my ABC");
        }
    }
    // __remote_value = atoi(payload);
    __got_remote_value = true;
    Logger::Debug("ConcernSensorSetter::onGot_MqttMessage");
    Logger::Print(this->_mqtt_topic, payload);
    Logger::Print("__remote_value", __remote_value);
}

