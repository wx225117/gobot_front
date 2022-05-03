
#                        ^
#      A1                |            A19
#     (-200.25, 589.75)  |            (200.25, 589.75)
#                        |
#                        |
#                        |
#                        |
#      T1                |            T19
#     (-200.25, 161.25)  |            (200.25, 161.25)
#                        Head(0,88)
#                        |
#------------O---------(0,0)---------O-------------->X

# from config.message_logger import MessageLogger

import sys
sys.path.append('D:\\XumingSource\\gobot_front')  # For runing in VsCode on Windows-10 

from Pylib.robot_map import MapSite
from gogame.arm_map import ArmMapSite_Catalog, ArmMapSites
from gogame.chessboard_cell import ChessboardCell
from gogame.human_level_robot_base import HumanLevelRobotBase
from Pylib.rabbit_mq_helper import AMQ_ConnectionConfig, g_amq
import enum


class ArmEEF(enum.Enum):
    SLEEP = 0
    LOAD = 1
    UNLOAD = 2
    MOVE_Z_TOP = 3
    MOVE_Z_MIDDLE = 4
    MOVE_Z_BOTTOM = 5


class HumanLevelGobotArm(HumanLevelRobotBase):

    def __init__(self,robot_serial_id:int, do_home=True) -> None:
        super().__init__()
        mq_name = 'gobot_xnnnn_arm'
        self.mq_name = mq_name.replace('nnnn', str(robot_serial_id))
        if do_home:
            self.HomeAaphaBeta()

    def HomeAaphaBeta(self):
        print('[Info] HumanLevelGobotArm::Home() ')
        commands = ['G28AI','G28BI', 'M996']
        g_amq.PublishBatch(self.mq_name, commands)


    def action_pickup_stone_from_cell(self, cell:ChessboardCell):
        pass

    def action_place_stone_to_trash_bin(self):
        pass

    # TODO:  Base robot can understand RobotMap, and ArmMap, HouseMap are the subclass
    def Pickup_Place(self, from_where:MapSite, to_where:MapSite, auto_park=False):
        # print("[Error]  human_level_gobot_arm.py Pickup_Place(), Not implicated. ")
        # return
        print("[Info] HumanLevelGobotArm::Pickup_Place from = (", from_where.Name + ")")
        self.MoveTo(from_where)
        self.EEF_Does(ArmEEF.MOVE_Z_BOTTOM)
        self.EEF_Does(do_load=ArmEEF.LOAD)
        self.EEF_Does(do_load=ArmEEF.MOVE_Z_TOP)

        print("[Info] HumanLevelGobotArm::Pickup_Place to = (", to_where.Name + ")")
        self.MoveTo(to_where)
        self.EEF_Does(do_load=ArmEEF.UNLOAD)
        self.EEF_Does(do_load=ArmEEF.SLEEP)
        if auto_park:
            parking_at = ArmMapSites().GetSingleSite(ArmMapSite_Catalog.PARKING)
            self.MoveTo(parking_at)
        g_amq.Publish(self.mq_name, 'M996')


    def MoveTo(self, site:MapSite ):
        x = site.X
        y = site.Y
        gcode = 'G1X' + str(x) + 'Y' + str(y)
        g_amq.Publish(self.mq_name, gcode)
        print(gcode)

    
    def EEF_Does(self, do_load:ArmEEF):
        if do_load==ArmEEF.SLEEP:
            g_amq.Publish(self.mq_name, 'M123S0')
        elif do_load==ArmEEF.LOAD:
            g_amq.Publish(self.mq_name, 'M123S200')

    def DisableMotor(self):
        g_amq.Publish(self.mq_name, 'M84')

    def Test_Eef(self):
        commands = ['M123P1S128', 'G4S1', 'M123P1S0', 'G4S5', 'M996']
        g_amq.PublishBatch(self.mq_name, commands)

    def Calibrate_1_HomeAlpha(self):
        '''
        Home, (AntiClockwise)-180, pause, -90, pause
        '''
        pause = "G4S5"
        commands = ['G28AI', 'G1A-6.283', pause, 'G1A-1.571', pause, 'M996']
        g_amq.PublishBatch(self.mq_name, commands)

    def Calibrate_2_HomeBeta(self):
        '''
        Home, (Clockwise) 180, pause, 90, pause 
        '''
        pause = "G4S5"
        commands = ['G28BI', 'G1B3.142', pause, 'G1B1.571', pause, 'M996']
        g_amq.PublishBatch(self.mq_name, commands)
        
    def Calibrate_3_A1_T19(self):
        'Home, Move to A1, pause, Move to T19, pause'
        pause = "G4S5"
        self.HomeAaphaBeta()

        cell_a1 = ChessboardCell()
        cell_a1.from_name("A1")
        a1_site = ArmMapSites().GetSingleSite(ArmMapSite_Catalog.CHESSBOARD_CELL, cell_a1)
        cell_t19 =  ChessboardCell()
        cell_t19.from_name("T19")
        t19_site = ArmMapSites().GetSingleSite(ArmMapSite_Catalog.CHESSBOARD_CELL, cell_t19)
        
        for i in range(5):
            commands = [ 'G1X' + str(a1_site.X) + "Y" + str(a1_site.Y)
                        ,pause
                        ,'G1X' + str(t19_site.X) + "Y" + str(t19_site.Y)
                        ,pause
                        ]  
            g_amq.PublishBatch(self.mq_name, commands)

if __name__ == '__main__':
    config = AMQ_ConnectionConfig()
    g_amq.ConnectToRabbitMq(config)

    arm = HumanLevelGobotArm(robot_serial_id=2134, do_home=False)

    arm.Calibrate_1_HomeAlpha()
    arm.Calibrate_2_HomeBeta()

    if True:
        # Test Map
        from_where = ArmMapSites().GetSingleSite(ArmMapSite_Catalog.HOUSE_VENDOR)
        to_cell = ChessboardCell()
        to_cell.from_name("A1")

        to_site = ArmMapSites().GetSingleSite(ArmMapSite_Catalog.CHESSBOARD_CELL,to_cell)
        arm.Pickup_Place(from_where=from_where, to_where=to_site)

    if False:
        # Test rabbitMQ

        for i in range(200):
            arm.Test_Eef()

        while True:
            arm.SpinOnce()