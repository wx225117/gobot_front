#include "button_gpio_ver2022.h"
#include "Arduino.h"
#include "von/cpp/utility/logger.h"
// #include "MyLibs/mqtt/wifi_mqtt_client.h"
#include "MyLibs/mqtt/wifi_mqtt_client.h"
// #include "von/cpp/utility/logger.h"

Button_Gpio::Button_Gpio(int gpio_pin_number){
    __pin_number = gpio_pin_number;
    pinMode(__pin_number, INPUT_PULLUP);
}


void Button_Gpio::__mqtt_publish_state(bool is_on){
    if ((millis() - __last_publish_timestamp) < ( __setting_mqtt_publish_interval_ms)){
        // Logger::Print("Button_Gpio::__mqtt_publish_state(),  too fast to publish mqtt", last_publish_timestamp);
        // Logger::Print("millis()", millis());
        // Logger::Print("__setting_mqtt_publish_interval_ms", __setting_mqtt_publish_interval_ms);
        return;
    }
    // Logger::Print("going to publishing ", is_on);
    if(is_on) {
        if (__setting_mqtt_publish_on_event){
            g_mqttClient.publish(_mqtt_publish_topic, 2, true,_PAYLOAD_STRING_ON.c_str());
            __last_publish_timestamp = millis();
        }
    }else{
        if (__setting_mqtt_publish_off_event){
            g_mqttClient.publish(_mqtt_publish_topic, 2, true,_PAYLOAD_STRING_OFF.c_str());
            __last_publish_timestamp = millis();
        }
    }
}

// check button state, might publish mqtt message when its state is changed.
void Button_Gpio::SpinOnce(){
    bool is_pressed = ! digitalRead(__pin_number);
    // Logger::Debug("Button_Gpio::SpinOnce()");
    // Logger::Print("__pin_number", __pin_number);
    // Logger::Print("is_pressed", is_pressed);
    if (is_pressed != __last_state_is_pressed){
        // Logger::Print("Button_Gpio::SpinOnce()  is_pressed", is_pressed);
        // if (__callback != nullptr){
        if (true){
            bool go_on_remote = true;  // TODO:   *__callback();
            // *__callback();
            if (go_on_remote){
                if(_mqtt_publish_topic != nullptr){
                    // Logger::Print("Button_Gpio::SpinOnce()  is_pressed", is_pressed);
                    __mqtt_publish_state(is_pressed);
                }
            }
        }
    } 
    __last_state_is_pressed = is_pressed;
}


bool Button_Gpio::IsPressed(){
    SpinOnce();
    return __last_state_is_pressed;
}