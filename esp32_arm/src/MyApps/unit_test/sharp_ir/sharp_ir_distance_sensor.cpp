#include "all_applications.h"
// #include "all_devices.h"
#ifdef I_AM_SHARP_IR_DISTANCE_SENSOR
#include "CNC/Sensor/sharp_ir_distance_adc.h"
#include <HardwareSerial.h>


#define ir 12
#define model 20150
// ir: the pin where your sensor is attached
// model: an int that determines your sensor:  1080 for GP2Y0A21Y
//                                            20150 for GP2Y0A02Y
//                                            (working distance range according to the datasheets)

SharpIrAdc sensor(ir, model);

void setup() {
  Serial.begin(115200);
}

void loop() {
  delay(2000);   
  unsigned long pepe1=millis();  // takes the time before the loop on the library begins
  int dis=sensor.distance();  // this returns the distance to the object you're measuring

  Serial.print("Mean distance: ");  // returns it to the serial monitor
  Serial.println(dis);
  
  unsigned long pepe2=millis()-pepe1;  // the following gives you the time taken to get the measurement
  Serial.print("Time taken (ms): ");
  Serial.println(pepe2);  
}
#endif