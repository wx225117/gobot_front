#include "all_devices.h"
#ifdef I_AM_GOBOT_HOUSE

#include "gobot_house.h"
#include "gobot_house_hw.h"
#include "MyLibs/MyFunctions.hpp" 
#include "IoT/mqtt_syncer.h"
#include "IoT/main_mqtt.h"

GobotHouse* robot; 
GobotHouseHardware* robot_hardware;
GcodeQueue* gcode_queue;
MessageQueue* mqtt_message_queue;

Stepper motor(PIN_ALPHA_STEP_2109, PIN_ALPHA_DIR_2109);       // STEP pin: 2, DIR pin: 3
StepControl controller;    // Use default settings 


// void loop_motor() 
// {
//   motor.setTargetRel(1000);  // Set target position to 1000 steps from current position
//   controller.move(motor);    // Do the move
//   delay(500);
// }
// void setup_enable_motor(){
//     pinMode(PIN_ALPHA_ENABLE, OUTPUT);
//     pinMode(PIN_BETA_ENABLE, OUTPUT);
//     digitalWrite(PIN_ALPHA_ENABLE, LOW);
//     digitalWrite(PIN_BETA_ENABLE, LOW);
// }

void setup_robot_hardware(){
    SingleAxisHomer* alpha_homer = new SingleAxisHomer(PIN_HOME_ALHPA_2109, LOW);
    SingleAxisHomer* beta_homer = new SingleAxisHomer(PIN_HOME_BETA_2109, LOW);
    Stepper* alpha_stepper = new Stepper(PIN_ALPHA_STEP_2109, PIN_ALPHA_DIR_2109);
    Stepper* beta_stepper = new Stepper(PIN_BETA_STEP_2109, PIN_BETA_DIR_2109);
    
    robot_hardware = &GobotHouseHardware::getInstance();
    robot_hardware->LinkHomer(alpha_homer, beta_homer);
    robot_hardware->LinkStepper(alpha_stepper, beta_stepper);
    
}

void setup(){
    Serial.begin(115200);
    Serial.println("Hi Xuming, I am your lovely bot,  GobotHouse. ");

    setup_robot_hardware();
    robot = &GobotHouse::getInstance();
    gcode_queue = new GcodeQueue();
    robot->Setup();
    robot->LinkLocalGcodeQueue_AsProducer(gcode_queue);
    robot_hardware->LinkLocalGcodeQueue_AsConsumer(gcode_queue);

    // mqtt, bridge, receiver.
    setup_mqtt_block_connect();
    mqtt_message_queue = new MessageQueue();
    String mqtt_topic = "gobot/xROBOT_SERIAL_ID/house";
    mqtt_topic.replace("ROBOT_SERIAL_ID",String(ROBOT_SERIAL_ID));
    append_mqtt_bridge(mqtt_topic.c_str(), mqtt_message_queue, robot); 
    setup_mqtt_on_message_receive(); 
    Serial.println("lovely bot,  GobotHouse.  setup() is done.  Good luck!");
}

void loop(){
	robot->SpinOnce();
    robot_hardware->SpinOnce();
    loop_mqtt();
}


#endif