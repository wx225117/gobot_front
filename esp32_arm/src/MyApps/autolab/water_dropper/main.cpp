#include "MyLibs/utility/logger.h"
#include "board/web_configurator_diction.h"
#include "MyLibs/utility/webserver_starter/webserver_starter.h"

#include "board/board.h"
#include "lua/app_lua.h"
#include "app.h"

#include "../select_app.h"
#ifdef I_AM_WATER_DROPPER

WaterDropper_App app;
WaterDropper_LuaWrapper lua_wrapper;

#define PIN_ENCODER_A 26
#define PIN_ENCODER_B 27

Encoder encoder = Encoder(PIN_ENCODER_A, PIN_ENCODER_B, 20 );
void doA(){encoder.handleA();}
void doB(){encoder.handleB();}
void init_encoder(){
	encoder.quadrature = Quadrature::ON;
	encoder.pullup = Pullup::USE_INTERN;
	encoder.init();
	encoder.enableInterrupts(doA, doB);
}

WaterDropper_WebConfiguratorDiction diction;
void setup(){
	water_dropper_board.Init("I_AM_WATER_DROPPER");
	init_encoder();
	water_dropper_board.LinkEncoder(&encoder);

	diction.Init(water_dropper_board.Get_WebConfigButton());
	WebServerStarter::Begin(&diction);
	setup_wifi_mqtt_blocking_mode();  //TODO:  connect to wifi once.
	app.Link_Mqtt_to_TextMessageQueue("water_dropper/command");
	app.Link_lua_from_File(&lua_wrapper, "/test.lua");
	lua_wrapper.Link_Mqtt_for_Test("lua/test");
}

void loop(){
	encoder.update();
	app.SpinOnce();
}

#endif
