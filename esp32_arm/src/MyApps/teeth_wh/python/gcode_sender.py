from von.rabbitmq_agent import g_amq


class GcodeSender():

    def __init__(self) -> None:
        self.target_device_id = 221109

    def send_code(self, code):
        exchange_name = 'twh'
        queue_name = exchange_name + '_' + str(self.target_device_id) + '_gcode'  # twh_221109_gcode
        g_amq.Publish(exchange_name, queue_name, code)

    def home_alpha(self):
        self.send_code('G28a')

    def home_y(self):
        self.send_code('G28Y')

    def move_centerbox_to_outside(self):
        self.send_code('G1X1200')

    def move_centerbox_to_inside(self):
        self.send_code('G1X400')

    def pickup_from_centerbox(self):
        # move to top
        self.send_code('G1Z0')
        self.enable_vacuum_sucker(True)
        # arm to center position
        self.send_code('G6a0')
        # move down
        self.send_code('G1Z100')
        self.send_code('G4S2')
        # move up
        self.send_code('G1Z0')
        self.send_code('G4S2')
        self.send_code('M408')
        
    def drop_to_cellbox(self, row, col):
        self.move_xy_to(row * 40, col * 40)
        self.enable_vacuum_sucker(False)

    def move_z_top(self):
        self.send_code("G1Z0")

    def EefMove_Z_toMiddle(self, depth:int):
        self.send_code("G1Z30")

    def Eef_TurnOn_VacuumFan(self, is_turn_on: bool):
        if is_turn_on:
            self.send_code('M42P33S0')
        else:
            self.send_code('M42P33S1')

    def enable_vacuum_sucker(self, is_enable:bool):
        # print('gcode ','Eef_EnableSuck')
        if is_enable:
            self.send_code('M280P0S0')   # servo 0 angle 0
        else:
            self.send_code('M280P0S270')  # servo 0 angle 270
            
    def dwell(self, second: int):
        self.send_code('G4S' + str(second))

    def move_arm_to_center(self):
        self.move_a_to(0)

    def move_a_to(self, angle_in_rad):
        self.send_code("G6a" + str(angle_in_rad) )

    def move_xy_to(self, x, y):
        # print('gcode ','MoveTo')
        self.send_code("G1X" + str(x) + "Y" + str(y))