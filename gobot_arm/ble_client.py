 from bluepy import btle
 
 print ("Connecting...")
 dev = btle.Peripheral("B0:B4:48:BF:C9:83")
 
 print ("Services...")

 for svc in dev.services:
     print (str(svc))

 lightSensor = btle.UUID("f000aa70-0451-4000-b000-000000000000")
 
 lightService = dev.getServiceByUUID(lightSensor)
 for ch in lightService.getCharacteristics():
     print (str(ch))

 import time
 import binascii
 uuidConfig = btle.UUID("f000aa72-0451-4000-b000-000000000000")
 lightSensorConfig = lightService.getCharacteristics(uuidConfig)[0]
 # Enable the sensor
 lightSensorConfig.write(bytes("\x01"))
 
 time.sleep(1.0) # Allow sensor to stabilise
 
 uuidValue  = btle.UUID("f000aa71-0451-4000-b000-000000000000")
 lightSensorValue = lightService.getCharacteristics(uuidValue)[0]
 # Read the sensor
 val = lightSensorValue.read()
 print ("Light sensor raw value", binascii.b2a_hex(val))




