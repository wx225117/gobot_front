from von.remote_var_mqtt import RemoteVar_mqtt
from wcs_robots.gcode_sender import GcodeSender, all_gcode_senders
from bolt_nut import PickingPacking_Tooth

class TwhRobot_LoopPorter():

    def __init__(self, robot_id:str, row_id:int) -> None:
        self.id = row_id
        self.robot_id = robot_id
        self.state_topic = "twh/" + robot_id + '/r' + str(row_id) + "/state"  #'twh/221109/r0/state'
        self.state = RemoteVar_mqtt(self.state_topic, 'idle')

        gcode_topic = "twh/" + robot_id + '/r' + str(row_id) + "/gcode"  #'twh/221109/r0/state'
        self.gcode_sender = GcodeSender(gcode_topic)
        all_gcode_senders.append(self.gcode_sender)
        self.target_tooth = None
        self.__target_layer = 0

    # def port_to_pick(self, tooth: PickingPacking_Tooth) -> None:
    def port_to_pick(self, target_col:int, target_layer:int) -> None:
        self.state.set('moving')    # set to 'moving' when gcode-G1 is sent. ??
        # self.target_tooth = tooth
        self.__target_layer = target_layer
        # print("TwhRobot_Row::move_to()", 'row, col, layer = ' ,self.id, tooth.col, tooth.layer )
        
        mcode ='M42P99S1'  # turn off all green leds
        self.gcode_sender.append_gmcode_to_queue(mcode)

        gcode = 'G1X' + str(target_col)
        self.gcode_sender.append_gmcode_to_queue(gcode)

        mcode = 'M408' + self.state_topic
        self.gcode_sender.append_gmcode_to_queue(mcode)

        mcode ='M999'
        self.gcode_sender.append_gmcode_to_queue(mcode)

    def show_layer_led(self):
        mcode = 'M42P' + str(self.__target_layer) + 'S1'
        self.gcode_sender.append_gmcode_to_queue(mcode)

        mcode ='M999'
        self.gcode_sender.append_gmcode_to_queue(mcode)

    def turn_off_leds(self):
        mcode = 'M42P99S1'
        self.gcode_sender.append_gmcode_to_queue(mcode)

        mcode ='M999'
        self.gcode_sender.append_gmcode_to_queue(mcode)  

