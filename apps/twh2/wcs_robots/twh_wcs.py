from wcs_robots.twh_robot_loop_porter import TwhRobot_LoopPorter
from wcs_robots.twh_robot_packer import TwhRobot_Packer
from wcs_robots.gcode_sender import gcode_senders_spin_once
from business_logical.withdraw_order import OrderTaskTooth, OrderTaskManager, OrderTask
from database.db_withdraw import DB_OrderTask

import multiprocessing
from von.remote_var_mqtt import RemoteVar_mqtt
from von.mqtt_agent import g_mqtt,g_mqtt_broker_config
import time
from logger import Logger




class Twh_WarehouseControlSystem():

    def __init__(self, queue_deposit:multiprocessing.Queue) -> None:
        # 1. Create port-robot list
        self.__porters = [TwhRobot_LoopPorter('221109', 0)]
        for i in range(3):
            new_porter = TwhRobot_LoopPorter('221109', i+1)
            self.__porters.append(new_porter)
        self.__order_task_manager = OrderTaskManager()
        self.__packer = TwhRobot_Packer()
        # 2. Create packer_robot

        # 3. Create queues
        # When there is a request for picking,  put the tooth into the queue.
        # when chain loop start moving, the teeth will be removed.
        # self.__withdraw_request_queue = []  
        # self.__fake_loopPorter = TwhRobot_LoopPorter('', -1)
        # self.__picking_ready_porter = self.__fake_loopPorter

        # __button_pick is a green button sit on packer.
        self.__button_pick = RemoteVar_mqtt('twh/221109/packer/button/pick','idle')

        # __button_pack is a blue button sit on packer.
        self.__button_pack = RemoteVar_mqtt('twh/221109/packer/button/pack','idle')

        self.__wcs_state = 'idle'

    def OrderTask_Link_PackerCell(self):
        '''
        Only assign one packer_cell (the idle one) for each running.
        For the order(multi teeth) ,will move the queue from database to WCS buffer.
        '''
        pass
 


    def Find_LoopPorter_ready(self) -> TwhRobot_LoopPorter:
        for porter in self.__porters:
            if porter.GetState() == 'ready':
                return porter
        return None # type: ignore

    def all_loop_porter_are_idle(self) -> bool:
        for porter in self.__porters:
            if porter.GetState() != 'idle':
                return False
        return True
    
    # def PickPlace_PortPair(self) -> None:
    #     '''
    #     Step 1
    #         Try to finish a picking-placing task,  Then, We can get an idle loop_porter.

    #     Step2
    #         Only create one picking-placing task (from one of the idle loop-porters) for each running.
    #         1. find an idle row-porter.
    #         2. find tooth from order , and the tooth is bound to the idle loop-porter we found.
    #         3. find connected shipout_box from the tooth
    #         4. start to move the row-porter.
    #     '''

    #     # Logger.Debug("Twh_WarehouseControlSystem::PickPlace_PortPair()")
    #     # if self.__state == 'picking_packing':
    #     if not self.__picking_ready_porter.IsNone:
    #         self.TryTo_end_one_pick_place_task()

    #     # Find all idle loop_porters.
    #     idle_porters = self.Find_LoopPorter_idle()
    #     if idle_porters is None:
    #         return

    #     # Find any tooth inside idle loop_porters
    #     target_tooth = self.Find_Tooth_in_porters()
    #     if target_tooth is None:
    #         return
        
    #     # Start picking-packing
    #     porter.('picking_packing')
    #     porter.show_layer_led()
    #     self.__packer.show_pack_box_led('green', porter.target_tooth.pack_cell_id)
    #     # wait operator to press the button
    #     self.__button_pick.set('OFF')
    #     self.__picking_ready_porter = porter
    #     self.__state = 'picking_packing'





    #     # if any porter is ready for picking_packing, do it.
    #     for porter in self.__porters:
    #         # Logger.Print('Twh_WarehouseControlSystem::PickPlace_PortPair()   porter.state.get() ', porter.state.get())
    #         if porter.target_tooth is not None:
    #             # Logger.Print('Twh_WarehouseControlSystem::PickPlace_PortPair()  porter.id', porter.id)
    #             if porter.state.get() == 'ready':
    #                 Logger.Print('Twh_WarehouseControlSystem::PickPlace_PortPair()  Porter is ready. porter_id', porter.id)
    #                 if self.__state == 'idle':
    #                     # show green led on porter, and on packer
    #                     porter.state.set('picking_packing')
    #                     porter.show_layer_led()
    #                     self.__packer.show_pack_box_led('green', porter.target_tooth.pack_cell_id)
    #                     # wait operator to press the button
    #                     self.__button_pick.set('OFF')
    #                     self.__picking_ready_porter = porter
    #                     self.__state = 'picking_packing'
    #                 return
        
    #     # TODO:   before do below,  must check there is a ready porter.
    #     portable_tooth =  self.___withdraw_teeth_queue_get_portable()
    #     if portable_tooth is None:
    #         return

    #     portable_tooth.print_out("Portable idle_porter and portable_tooth ")
    #     idle_porter = self.__porters[portable_tooth.row]
    #     idle_porter.target_tooth = portable_tooth   # assign portable_tooth to loop-porter
    #     idle_porter.port_to_pick(portable_tooth.col, portable_tooth.layer)
    #     idle_porter.show_layer_led()
    #     # self.__state = 'moving'  #???
    #     # print('Twh_WarehouseControlSystem::PickPlace_PortPair()  Found target_tooth ', portable_tooth.row, portable_tooth.col, portable_tooth.layer)
    #     self.__withdraw_request_queue.remove(portable_tooth)   # If move this line after packing,  will be takeout from the queue repeatly?

    def Do_deposit_begin(self, new_deposit_request):
        Logger.Info("Twh_WarehouseControlSystem::Do_deposit()")
        Logger.Print("new_deposit_request", new_deposit_request)
        # the loop-porter will move to col-position
        row_id = new_deposit_request['row']
        col_id = new_deposit_request['col']
        layer_id = new_deposit_request['layer']
        Logger.Print("row_id", row_id)
        Logger.Print("porters count", len(self.__porters))
        porter = self.__porters[row_id]
        self.__depositing_porter = porter
        Logger.Print('layer_id', layer_id)
        porter.MoveTo(col_id, layer_id)
        porter.show_layer_led()
        Logger.Print("Twh_WarehouseControlSystem::Do_deposit()    point", 99)

    def Do_deposit_end(self):
        self.__depositing_porter.turn_off_leds()
        self.__depositing_porter.SetStateTo('idle')
    
    def __Withdraw_Pair_porter_tooth(self):
        # Logger.Debug("Twh_WarehouseControlSystem::__Withdraw_Pair_porter_tooth()")
        for porter in self.__porters:
            if porter.GetState() == 'idle':
                # Logger.Print('found idle porter, porter_id',porter.id)
                tooth = self.__order_task_manager.FindTooth_is_in_porter(porter.id)
                if tooth is not None:
                    Logger.Print('Paired.   found tooth is in the porter, col', tooth.col)
                    return porter, tooth
        return None, None


    def state_machine_main(self, queue_web_request:multiprocessing.Queue):
        if self.__wcs_state == 'idle':
            if queue_web_request.empty():
                self.__wcs_state = 'withdraw_dispaching'
            else:
                new_request = queue_web_request.get()
                if new_request['message_type'] == 'deposit_begin':
                    self.Do_deposit_begin(new_request)
                else:
                    Logger.Error('state_machine_main(),   wrong deposite request')
                self.__wcs_state = 'deposit_begin'
        
        if self.__wcs_state == 'deposit_begin':
                #user is operating, 
                # when he/she is complete the task, 
                # he/she will press a web page button. 
                # and queue_web_requst will create a new message
                if queue_web_request.empty():
                    return
                new_request = queue_web_request.get()   # will be blocked if queue is empty
                if new_request['message_type'] == 'deposit_end':
                    self.Do_deposit_end()
                else:
                    Logger.Error('state_machine_main() try to end deposit ,   wrong deposite request')
                    Logger.Print('new_request["message_type"]',new_request['message_type'])
                self.__wcs_state = 'idle'
    
        if self.__wcs_state == 'withdraw_dispaching':
            if self.all_loop_porter_are_idle() and self.__order_task_manager.GetTasksCount() == 0:
                self.__wcs_state = 'idle'
                return
            # try to find idle_porter matched tooth in order. and pair (porter, tooth)
            # Logger.Debug("state_machine_main()  withdraw_dispaching ")
            idle_porter, picking_tooth = self.__Withdraw_Pair_porter_tooth()
            if idle_porter is not None:
                # assign tooth to idle porter
                idle_porter.SetPortingTooth(picking_tooth)
                idle_porter.MoveTo(picking_tooth.col, picking_tooth.layer)

            # try to find ready_porter
            ready_porter = self.Find_LoopPorter_ready()
            if ready_porter is None:
                # all porters are busy
                return
            ready_porter.show_layer_led()
            porting_tooth = ready_porter.GetPortingTooth()
            self.__packer.turn_on_cell_led('green', porting_tooth.layer)

            self.__picking_ready_porter = ready_porter
            self.__wcs_state = 'picking_placing'
                    
        if self.__wcs_state == 'picking_placing':
            if self.__button_pick.get() == 'ON':
                porting_tooth = self.__picking_ready_porter.GetPortingTooth()
                # porting_tooth.TransferTo('packer')
                self.__order_task_manager.Transered_into_Packer(porting_tooth)
                self.__picking_ready_porter.turn_off_leds()
                self.__picking_ready_porter.SetStateTo('idle')
                self.__packer.turn_off_all_led('green')


                self.__wcs_state = 'withdraw_dispaching'


    def SpinOnce(self, deposit_queue:multiprocessing.Queue) ->str:
        '''
        return:  __wcs_state
        '''
        self.__order_task_manager.renew_orders_from_database()
        self.__order_task_manager.remove_packed_order()

        self.OrderTask_Link_PackerCell()
        self.state_machine_main(deposit_queue)

        return self.__wcs_state

def WCS_Main(deposit_queue:multiprocessing.Queue):
        g_mqtt_broker_config.client_id = '20221222'
        g_mqtt.connect_to_broker(g_mqtt_broker_config,wait_connected=True)                # DebugMode, must be turn off.  
        wcs = Twh_WarehouseControlSystem(deposit_queue)
        showing_wcs_state = ''

        while True:
            wcs_state =  wcs.SpinOnce(deposit_queue)
            gcode_senders_spin_once()
            if showing_wcs_state != wcs_state:
                showing_wcs_state = wcs_state
                g_mqtt.publish('twh/221109/wcs_state',showing_wcs_state)
            time.sleep(0.5)


# from wms to wcs
wcs_deposit_queue = multiprocessing.Queue()         

def Start_WCS_Process():
    # p = multiprocessing.Process(target=WCS_Main, args=(wcs_deposit_queue, set_packer_cell_state_queue, packer_cells_state))
    p = multiprocessing.Process(target=WCS_Main, args=(wcs_deposit_queue,))
    p.start() 
    Logger.Info('WCS is running on new process.....')

    # https://pymotw.com/2/multiprocessing/communication.html#communication-between-processes




