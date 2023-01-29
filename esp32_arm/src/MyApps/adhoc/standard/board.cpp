#include "board.h"



void Adhoc_RouterBoard::__InitHardware(){
    // init gpio.
    pinMode(PIN_STATE_LED, OUTPUT);
}

void Adhoc_RouterBoard::Init(){
    Serial.begin(115200);
    Serial.print("\n\n\n\n\n\n");
    Logger::Info("==========================================");
    Logger::Info("Hi Xuming, I am Mesh-Adhoc Router or Sniffer. Good luck......");
    Logger::Info("==========================================");
    Serial.print(FCBC_RESET);
    this->__InitHardware();

    this->RepportRamUsage();
}

