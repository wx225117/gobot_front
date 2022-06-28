#include "logger.h"
#include <HardwareSerial.h>


void Logger::PrintTitle(const char* title){
    Serial.print(FORE_YELLOW);
    Serial.print(BGC_BLUE);
    Serial.print(title);
    Serial.print(BGC_BLACK);
    Serial.println(FORE_GREEN);
}