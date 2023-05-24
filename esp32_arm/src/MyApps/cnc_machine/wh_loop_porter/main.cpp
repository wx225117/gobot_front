
#include "board/board.h"
#include "loop_porter_app.h"
#include "robot/loop_porter_robot.h"
#include "Mqtt/remote_binary_output_group.h"
#include "all_applications.h"

#ifdef I_AM_TEETH_WAREHOUSE_LOOP_PORTER
#include "app_config/twh_loop_porter.h"

#define LEDS_COUNT 7

Twh_LoopPorter_Board board;
Twh_LoopPorter_Robot robot;
Twh_LoopPorter_App app(MY_ROBOT_ROW_ID);
RemoteLedsState leds;

void test(){
    // board.TestLeds(200);
    // board.GetNumberDisplayer()->Test(9999, 1);

    // gs_PositionTrigger_Array::Instance().Test_PositionTriggers(99);
    // gs_CncActuator_List::Instance().GetActuator(0).test
    // board.Test_Stepper(999);
    
    // float xx = Twh2_Circleloop_Armsolution_Config().Slope_Steps_per_box();
}

void setup(){
    board.Init();

    setup_wifi_mqtt_blocking_mode();

    robot.Init(&board, MQTT_TOPIC_FOR_HOME_POSITION);
    GcodeQueue* gcode_queue = robot.GetGcodeQueue();
    app.Link_Mqtt_to_GcodeQueue(MQTT_TOPIC_GCODE, gcode_queue);

    app.LinkBoard(&board);
    Logger::Info ("App::loop-porter::setup() is done. ");

    gcode_queue->AppendGcodeCommand("G28X");
    gcode_queue->AppendGcodeCommand(MQTT_TOPIC_M408_REPORT_STATE_ON_SETUP);

    leds.Init(LEDS_COUNT);
    gs_MqttSubscriberManager::Instance().AddSubscriber(MQTT_TOPIC_FOR_LEDS, &leds);

}

void show_leds(){
    char* leds_command;  // 'N' => ON, 'F' => OFF
    leds_command = leds.Get();
    for(int i=0; i<LEDS_COUNT; i++){
        if (*(leds_command+i) == 'N')
            board.GetLed(i)->TurnOn();
        else
            board.GetLed(i)->TurnOff();
    }
}

void loop(){
    app.SpinOnce();
    robot.SpinOnce();
    robot.MySpinOnce();
    show_leds();
}

#endif
