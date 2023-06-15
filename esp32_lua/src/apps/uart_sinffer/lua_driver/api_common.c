#include "freertos/FreeRTOS.h"
#include "freertos/task.h"


#include <lua/lua.h>


// void (*__service_function)(const char*); 
// void def_callback(const char*);

void (*mqtt_publish) (const char*, const char*);
void (*mqtt_subscribe) (const char*);
int (*mqtt_read_payload) (const char*, char*);
void (*mqtt_release_buffer) (const char*);
const char* (*mqtt_read_first_topic)();

void set_callback_mqtt_publish( void (*service_function)(const char*, const char*) ){
	mqtt_publish = service_function;	
}
void set_callback_mqtt_subscribe( void (*service_function)(const char*) ){
	mqtt_subscribe = service_function;	
}
void set_callback_read_first_topic(const char* (*service_function)()){
	mqtt_read_first_topic = service_function;
}
void set_callback_mqtt_read_payload( int (*service_function)(const char*,char*) ){
	mqtt_read_payload = service_function;	
}
void set_callback_mqtt_release_buffer(void (*service_function) (const char*)){
	mqtt_release_buffer = service_function;
}

int LuaMqttSubscribe(lua_State* L){
	uint8_t topic[64];
	uint8_t semcount;
	
	//获得LUA传递过来的数组
	int length = 0;
	int i = lua_gettop(L);
	if(lua_istable(L, i)){
		lua_pushnil(L);
		
		while(lua_next(L, i) != 0){
			topic[length] = lua_tointeger(L, -1);
			lua_remove(L, -1);
			++length;
		}
	}
	mqtt_subscribe(topic);
	
	// do{
	// 	// Wait while UART TX is busy.
	// 	semcount = 0;
	// }while(semcount > 0);
	// uart_write_bytes_with_break(UART_NUM_1, buf, length, 1000);
	return 0;
}


int LuaMqttPublish(lua_State* L){
	uint8_t topic[64];
	uint8_t payload[100];
	
	//获得LUA传递过来的数组
	int length = 0;
	int i = lua_gettop(L);
	if(lua_istable(L, i)){
		lua_pushnil(L);
		
		while(lua_next(L, i) != 0){
			topic[length] = lua_tointeger(L, -1);
			lua_remove(L, -1);
			++length;
		}
	}

	length = 0;
	i = lua_gettop(L);
	if(lua_istable(L, i)){
		lua_pushnil(L);
		
		while(lua_next(L, i) != 0){
			payload[length] = lua_tointeger(L, -1);
			lua_remove(L, -1);
			++length;
		}
	}
	
	// do{
	// 	// Wait while UART TX is busy.
	// 	semcount = 0;
	// }while(semcount > 0);
	// uart_write_bytes_with_break(UART_NUM_1, buf, length, 1000);
	mqtt_publish(topic,payload);
	return 0;
}

int LuaMqttReadFirstTopic(lua_State* L){
	
}

int LuaMqttReadPayload(lua_State* L){
	char topic[64];
	char* payload;

	// copy from buffer to lua
	int length = mqtt_read_payload(topic, payload);
	// for(int i=0; i<length;i++){
		lua_pushstring(L, payload);
	// }
	// release origin buffer of payload
	mqtt_release_buffer(topic);

}


