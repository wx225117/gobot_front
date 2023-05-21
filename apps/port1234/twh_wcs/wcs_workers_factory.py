from twh_wcs.twh_robot.twh_loop_porter import Twh_LoopPorter
# from twh_wcs.twh_robot.twh_thames_bridge_packer import Twh_ThamesBridge_Packer
from twh_wcs.von.wcs.packer.simple_packer import SimplePacker

from twh_wcs.von.wcs.pick_placer.manual_pick_placer import Manual_PickPlacer
from twh_wcs.von.wcs.shipper.manual_shipper import Manual_Shipper

from twh_wcs.von.wcs.conveyor.tube_conveyor import TubeConveyor
from twh_wcs.von.wcs.porter.loop_porter import LoopPorter
from twh_wcs.von.wcs.pick_placer.pick_placer import Wsc_PickPlacerBase
from twh_wcs.von.wcs.packer.packer import Wcs_PackerBase
from twh_wcs.von.wcs.shipper.shipper import Wcs_ShipperBase

from von.logger import Logger

class WcsWorkers:
    # software: WarehouseBase
    warehouse_name = 'not named warehouse'
    loop_porters = list[LoopPorter]()
    tube_conveyors = list[TubeConveyor]()
    pick_placers = list[Wsc_PickPlacerBase]()
    packers = list[Wcs_PackerBase]()
    shippers = list[Wcs_ShipperBase]()

g_workers = dict[str, WcsWorkers]()

class WorkersFactory:

    @classmethod
    def FindIdlePackers(cls, warehouse_id:str) -> list[Wcs_PackerBase]:
        idle_packers = list[Wcs_PackerBase]()
        for packer in g_workers[warehouse_id].packers:
            if packer.GetState() == 'idle':
                idle_packers.append(packer)
        return idle_packers

    @classmethod
    def EachWorker_SpinOnce(cls):
        for workers in g_workers.values():
            for porter in workers.loop_porters:
                porter.SpinOnce()
                


    @classmethod
    def Create_WcsWorkers(cls, warehouse_id:str) -> WcsWorkers:
        wcs_workers = WcsWorkers()

        if warehouse_id == '221109':
            wcs_workers.warehouse_name = '某某义齿加工厂'
            for i in range(4):
                new_porter = Twh_LoopPorter(warehouse_id, i)
                wcs_workers.loop_porters.append(new_porter)
            
            new_picker = Manual_PickPlacer("twh/" + warehouse_id + 'picker')
            
            wcs_workers.pick_placers.append(new_picker)
            for i in range(12):
                new_packer = SimplePacker(i)
                WcsWorkers.packers.append(new_packer)

            new_shipper = Manual_Shipper("twh/" + warehouse_id + 'shipper/button')
            wcs_workers.shippers.append(new_shipper)

            g_workers[warehouse_id] = wcs_workers
            return wcs_workers

        elif warehouse_id == '230220':
            g_workers[warehouse_id] = wcs_workers
            wcs_workers.warehouse_name = '山东雅乐福义齿加工厂'
            for i in range(1):
                new_porter = Twh_LoopPorter(warehouse_id, i)
                wcs_workers.loop_porters.append(new_porter)
            for i in range(1):
                new_tube_conveyor = TubeConveyor(warehouse_id, 0 ,'','')
                wcs_workers.tube_conveyors.append(new_tube_conveyor)
            return wcs_workers
        

        else:
            Logger.Error("CreateWcsInstance()  Error")
            Logger.Print('wcs_instance_id', warehouse_id)
            return None # type: ignore