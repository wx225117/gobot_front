
#include "MyBleServerCallbacks.h"
#include <esp_log.h>
#include <HardwareSerial.h>


#define LOG_TAG "MyBLE connection."

void MyBleServerCallbacks::onConnect(BLEServer* pServer){
    // pMyNotifyTask->start();
    is_connected = true;
    // ESP_LOGW(LOG_TAG, "Client is connected! 11111111111 ");
    Serial.print("\nClient is connected! 11111111111 ");
}
void MyBleServerCallbacks::onDisconnect(BLEServer* pServer){
    // pMyNotifyTask->stop();
    is_connected = false;
    // ESP_LOGW(LOG_TAG, "Client is DIS-connected!");
    Serial.print("\nClient is DIS-connected!");
}

void MyBleServerCallbacks::onConnect(BLEServer* pServer, esp_ble_gatts_cb_param_t *param){
    is_connected = true;
    // ESP_LOGW(LOG_TAG, "Client is connected!  22222222222222");
    Serial.print("\nClient is connected!  22222222222222\n\n");
}