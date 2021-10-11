from commuDevice.ble_single_client import BleServerHead, BleSingleClient
from commuDevice.gcode_sender import GcodeFactory


class CornerAgent:
    def __init__(self, server_head:BleServerHead) -> None:
        self.buffer = []
        self.commu_device = BleSingleClient(server_head)

    def append_gcode(self, gcode:str) -> None:
        self.buffer.append(gcode)
        self.SpinOnce()

    def append_gcode(self, gcode:GcodeFactory) -> None:
        self.buffer.append(gcode.ToString())
        self.SpinOnce()

    def SpinOnce(self) -> None:
        self.commu_device.SpinOnce()
        if self.buffer.__len__() > 0:
            gcode = self.buffer[0]
            self.commu_device.write_characteristic(gcode)
            # double check: server side has received the gcode
            if True:
                self.buffer.remove(gcode)