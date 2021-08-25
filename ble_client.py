from bluepy import btle  
'''
sudo apt-get install libglib2.0-dev
pip3 install bluepy
sudo blescan # to verify bluepy
'''
from bluepy.btle import Scanner
from bluepy.btle import Peripheral, DefaultDelegate
'''
Alternative lib is bleak
https://github.com/hbldh/bleak
'''
import time
import logging

# https://anthonychiu.xyz/2016/04/05/communication-between-raspberry-pi-and-multiple-arduinos-via-bluetooth-low-power-ble/

class MyDelegate(DefaultDelegate):
    def __init__(self):
        DefaultDelegate.__init__(self)

    def handleNotification(self, cHandle, data):
        print('Notification is invoked')
        

class BleClient():
    def __init__(self, mac_addr):
        self.__arm_mac = 'b4:e6:2d:b2:f8:8f'
        self.__house_mac = '12:34:56:78:90:ab'
        # self.scan()

        ##self.dev = btle.Peripheral(self.__server_mac)
        # self.list_services_on_server()
        self.connect_to_server()
        # self.dev.

    def scan(self):
        scanner = Scanner()
        devices = scanner.scan(timeout = 3)
        print('Scan result:  Found %d devices in %d seconds------------' % (len(devices), 3))
        for dev in devices:
            print('Name', dev.getValueText(9))
            print('Address:', dev.addr)

    def list_services_on_server(self, server_mac):
        logging.info('Services on server  ------------------')
        self.dev = btle.Peripheral(server_mac)
        for svc in self.dev.services:
            print(str(svc))

    

    def connect_to_arm(self):
        self.arm.withDelegate(MyDelegate())
        svc = self.arm.getServiceByUUID('4fafc201-1fb5-459e-8fcc-c5c9c331914b')
        self.arm_info = svc.getCharacteristics('beb5483e-36e1-4688-b7f5-ea07361b26a8')[0]
        #self.house_info = svc.getCharacteristics('beb5483e-36e1-4688-b7f5-ea07361b26a8')[1]
        logging.info('ble connected to GATT server Arm !')

    def connect_to_house(self):
        self.house.withDelegate(MyDelegate())
        svc = self.house.getServiceByUUID('4fafc201-1fb5-459e-8fcc-c5c9c331914b')
        self.house_info = svc.getCharacteristics('beb5483e-36e1-4688-b7f5-ea07361b26a8')[0]
        #self.house_info = svc.getCharacteristics('beb5483e-36e1-4688-b7f5-ea07361b26a8')[1]
        logging.info('ble connected to GATT server House !')


    def write_characteristic(self, new_value):
        try:
            self.arm_info.write(bytes(new_value))
            logging.info('Updated charactoristic')
        #except bluepy.btle.BTLEDisconnectError:
        except:
            #self.dev.connect(self.__server_mac)
            print('ble_Write() Disconnected --> reconnecting')
            self.list_services_on_server()
            self.connect_to_server()
            #print('ble_write() Disconnected --> reconnecting  ')

    def read_characteristic(self):
        try:
            action_code = self.arm_info.read()
            return action_code
        #except bluepy.btle.BTLEDisconnectError:
        except:
            print("ble_read() Device disconnected-- reconnecting")
            self.list_services_on_server()
            self.connect_to_server()
            #continue
            action_code = self.arm_info.read()
            return action_code
        
    def spin_once(self):
        received = self.arm_info.read()
        logging.info('arm info: %s', received)

    def disconnect(self):
        self.dev.disconnect()
        time.sleep(1)

g_bleClient = BleClient()

if __name__ == '__main__':
    import signal        
    import sys
    logging.basicConfig(level=logging.DEBUG)
    logging.info('@@@@@@@@@@@@@@@@@@@@@@@@@')
    # runner = BleClient()
    def signal_handler(sig, frame):
        print("You pressed Ctrl+C")
        g_bleClient.disconnect()
        sys.exit(0)

    i = 65

    #if __name__ =='__main__':
    #print('@@@@@@@@@@@@@@@@@@@@@@@')
    signal.signal(signal.SIGINT, signal_handler)
    #signal.pause()


    #logging.basicConfig(level=logging.DEBUG)
    logging.info('practice BLE')
    #runner = BleClient()
    while True:
        time.sleep(1.7)
        i += 1
        if i> 128:
            i = 32
        g_bleClient.write_characteristic([i])






