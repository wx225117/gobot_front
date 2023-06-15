
extern "C"{
	extern void __main();
}

#include <Arduino.h>
#include "von_clib/task/wifi/task_wifi.h"
// #include "von_clib/task/mqtt/task_mqtt.h"
#include "lua_driver/task_mqtt.h"

TaskHandle_t* task_WiFi;
TaskHandle_t* task_Mqtt;

void setup(){
	xTaskCreate(TaskWiFi, "Wifi", 10000, NULL,  1, task_WiFi);   
	xTaskCreate(TaskMqtt, "Mqtt", 10000, NULL,  1, task_Mqtt);   

	__main();
}

void loop(){
	while(true){
		delay(100);
		// Serial.print(".");
	}
}


