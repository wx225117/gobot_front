
#include "board/board.h"
#include "loop_porter_app.h"
#include "robot/loop_porter_robot.h"
#include "all_applications.h"

#ifdef I_AM_TEETH_WAREHOUSE_LOOP_PORTER
#include "app_config/twh_loop_porter.h"

Twh_LoopPorter_Board board;
Twh_LoopPorter_Robot robot;
Twh_LoopPorter_App app(MY_ROBOT_ROW_ID);


void test_robot(){
    for (int i=0; i<6;i++){

        // gcode_queue.AppendGcodeCommand("G1X-90Y0");
        // gcode_queue.AppendGcodeCommand("G4S0");

        // gcode_queue.AppendGcodeCommand("G1X-90Y200");
        // gcode_queue.AppendGcodeCommand("G4S3");



        // app.gcode_queue.AppendGcodeCommand("G1X-190Y0");
        // app.gcode_queue.AppendGcodeCommand("G4S5");

        // app.gcode_queue.AppendGcodeCommand("G1X0Y0");
        // app.gcode_queue.AppendGcodeCommand("G4S5");

        // app.gcode_queue.AppendGcodeCommand("G1X190Y0");
        // app.gcode_queue.AppendGcodeCommand("G4S5");
    }
    
}

void test_arm(){
    // Twh2_Circleloop_ArmSolution arm;
    // FKPosition_XYZRPW input_fk;
    // input_fk.X = 123;
    // input_fk.Y = 234;
    // input_fk.Z = 12;
    // arm.Test_FK_MK_FK(&input_fk);

    // IKPosition_abgdekl input_ik;
    // input_ik.Positions[AXIS_ALPHA] = 456;
    // input_ik.Positions[AXIS_BETA] = -567;
    // arm.Test_IK_MK_IK(&input_ik);

    // MiddleKinematic input_mk;
    // input_mk.Arm_shaft_At_Y = -12.3f;
    // input_mk.Arm_Angle = -23.4f * DEG_TO_RAD;
    // arm.Test_MK_IK_MK(&input_mk);
    // arm.Test_MK_FK_MK(&input_mk);

    while (true){

    }
}

void setup(){
    board.Init();
    // board.TestLeds(200);
    // board.GetNumberDisplayer()->Test(9999, 1);

    // gs_PositionTrigger_Array::Instance().Test_PositionTriggers(99);
    // gs_CncActuator_List::Instance().GetActuator(0).test
    // board.Test_Stepper(999);
    
    // float xx = Twh2_Circleloop_Armsolution_Config().Slope_Steps_per_box();
    setup_wifi_mqtt_blocking_mode();

    robot.Init(&board, MQTT_TOPIC_FOR_HOME_POSITION);
    GcodeQueue* gcode_queue = robot.GetGcodeQueue();
    app.Link_Mqtt_to_GcodeQueue(MQTT_TOPIC_GCODE, gcode_queue);

    Logger::Info ("App::loop-porter::setup() is done. ");

    gcode_queue->AppendGcodeCommand("G28X");
    gcode_queue->AppendGcodeCommand(MQTT_TOPIC_M408_REPORT_STATE_ON_SETUP);
}

// int x= 8;
// String g1="G1";
void loop(){

    // GcodeQueue* gcode_queue = robot.GetGcodeQueue();
    // if(gcode_queue->GetFreeBuffersCount() >=6){
    //     g1 = "G1X";
    //     g1.concat(String(x));
    //     gcode_queue->AppendGcodeCommand(g1.c_str());
    //     x++;
    //     if(x>=50){
    //         x=8;
    //     }
    //     // Logger::Print("aaaaaaaaaaaaaaaaaaaaaa    xxxxxxxxxx", x);
    // }
    // Logger::Warn("Arduino loop() point 1");
    app.SpinOnce();
    // Logger::Warn("Arduino loop() point    2");

    robot.SpinOnce();
    // Logger::Warn("Arduino loop() point    3");

    robot.MySpinOnce();
    // Logger::Warn("Arduino loop() point    4");

    // delay(200);
}

#endif
