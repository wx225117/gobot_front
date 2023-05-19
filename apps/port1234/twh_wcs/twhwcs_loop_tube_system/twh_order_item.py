from twh_wcs.von.wcs.order_item import Wcs_OrderItemBase
from twh_database.db_withdraw_order import DB_WithdrawOrder
from twh_wcs.twhwcs_common.twh_robot_loop_porter import Twh_LoopPorter
from twh_wcs.von.wcs.conveyor.tube_conveyor import TubeConveyor


from von.logger import Logger

class Twh_OrderItem(Wcs_OrderItemBase):

    def __init__(self, db_doc_id:int) -> None:
        super().__init__(db_doc_id)
        # self.doc_id = db_doc_id
        self.DentalLocation = 'ur1'
        self.row :int
        self.col:int
        self.layer:int
        self.__linked_loop_porter:Twh_LoopPorter
        self.__linked_tube_conveyer:TubeConveyor
        self.__got_start_command = False


    def TransferToLocated(self, new_located:str, write_to_db:bool) -> None:
        self.__located = new_located
        if write_to_db:
            DB_WithdrawOrder.update_tooth_located(self.doc_id, new_located)

    
    def PrintOut(self, title:str):
        Logger.Info(title)
        Logger.Print('doc_id', self.doc_id)
        Logger.Print('DentalLocation', self.DentalLocation)
        Logger.Print('__located', self.__located)
        Logger.Print('row', self.row)
        Logger.Print('col', self.col)
        Logger.Print('layer', self.layer)

    def _SpinOnce(self):
        if self._state == 'idle':
            self._state = 'inside_loop_porter'
        if self._state == 'inside_loop_porter':
            if self.__got_start_command:
                loop_porter =  self._get_idle_porter()
                if loop_porter is not None:
                    loop_porter.MoveToGate()
                    self._state = 'inside_loop_porter_moving'
        if self._state == 'inside_loop_porter_moving':
            if self.__linked_loop_porter.GetState() == 'ready':
                self._state = 'loop_porter_gate'
        if self._state == 'loop_porter_gate':
            self.__linked_loop_porter.PickPlace()
            self._state = 'loop_porter_gate_picking'
        if self._state == 'loop_porter_gate_picking':
            if self.__linked_loop_porter.has_done():
                # Q: Need to wait tube conveyer be idle ?
                # A: Don't know now.
                self._state = 'loop_porter_done'

        if self._state == 'loop_porter_done':
            if self.__linked_tube_conveyer.GetState() == 'idle':
                self.__linked_loop_porter.SetStateTo('idle')
                self.__linked_tube_conveyer.SetVavle()
                self._state = 'tube_moving'
        if self._state == 'tube_moving':
                self._state = 'tube_outlet'
        if self._state == 'tube_outlet':
            if self.__linked_tube_conveyer.got_outlet():
                self.__linked_tube_conveyer.SetStateTo('idle')
                self._state = 'workstation'