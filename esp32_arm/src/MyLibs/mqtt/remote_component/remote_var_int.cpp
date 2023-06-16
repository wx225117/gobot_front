#include "remote_var_int.h"
#include "von/cpp/utility/logger.h"


void RemoteVar_Int::onGot_MqttMessage(const char* payload, uint16_t payload_len){
    __remote_value = atoi(payload);
    __got_remote_value = true;
    Logger::Debug("ConcernSensorSetter::onGot_MqttMessage");
    Logger::Print(this->_mqtt_topic.c_str(), payload);
    Logger::Print("__remote_value", __remote_value);
}

