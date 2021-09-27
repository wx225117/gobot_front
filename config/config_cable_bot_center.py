from commuDevice.ble_single_client import BleServerHead




class CornerBotFactory:
    def __init__(self) -> None:
        pass
        
    def CreateCorner_XPYP(self) -> BleServerHead:
        head = BleServerHead()
        head.AxisName ='A'
        head.BleDeviceName = "ConInt-XPYP"
        head.BleServiceUUID = "1fd03a14-1451-11ec-82a8-0242ac130003"
        head.BleCommuUUID = "4b7fbe96-1451-11ec-82a8-0242ac130003"
        head.BleStateUUID = "50711238-1451-11ec-82a8-0242ac130003"

    def CreateCorner_XNYP(self) -> BleServerHead:
        head = BleServerHead()
        head.AxisName='B'
        cBleDeviceName = "ConInt-XNYP"
        head.BleServiceUUID = "6909288a-1451-11ec-82a8-0242ac130003"
        head.BleCommuUUID = "6e0d5d7e-1451-11ec-82a8-0242ac130003"
        head.BleStateUUID = "72838810-1451-11ec-82a8-0242ac130003"

    # class XNYN:
    def CreateCorner_XNYN(self) -> BleServerHead:
        head = BleServerHead()
        head.AxisName='C'
        head.BleDeviceName = "ConInt-XNYN"
        head.BleServiceUUID = "77d37e60-1451-11ec-82a8-0242ac130003"
        head.BleCommuUUID = "8fdb27b0-1451-11ec-82a8-0242ac130003"
        head.BleStateUUID = "8175103c-1451-11ec-82a8-0242ac130003"

    # class XPYN:
    def CreateCorner_XPYN(self) -> BleServerHead:
        head = BleServerHead()
        head.AxisName='F'
        head.BleDeviceName = "ConInt-XPYN"
        head.BleServiceUUID  = "85f5ab94-1451-11ec-82a8-0242ac130003"
        head.BleCommuUUID = "96a1cd38-1451-11ec-82a8-0242ac130003"
        head.BleStateUUID  = "9c43b29c-1451-11ec-82a8-0242ac130003"
