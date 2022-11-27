
#include "m408_runner_report_json.h"
#include "Robot/axis_homer/home_trigger_array.h"

#include "IoT/wifi_mqtt_client.h"

#define PIN_IR_STATE 11
// Mqtt publish a message, report something via MQTT
// The payload is a static buffer, declared as global.
bool M408_Runner_ReportJson::StartExec(Gcode* gcode){
    // Logger::Debug("M408_Runner_ReportJson::Run()");

    __payload_string = "blocked";
    if (digitalRead(PIN_IR_STATE) == LOW) 
        __payload_string = "empty";   //TODO:  any memoory risk?
    const char* payload = __payload_string.c_str();
    // Logger::Print("payload", payload);
    mqttClient.publish("twh/221109/ir_state",2,true, payload);

    return true;   
}