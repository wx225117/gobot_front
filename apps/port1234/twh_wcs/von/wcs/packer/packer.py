from von.logger import Logger

class Wcs_PackerBase:

    def __init__(self) -> None:
        self._state = 'idle'

    def GetState(self) -> str:
        return self._state
    
    def SetStateTo(self, new_state:str):
        if new_state in ['idle', 'feeding']:
            self._state = new_state
        else:
            Logger.Error("Wcs_PackerBase:: SetStateTo()")
            Logger.Print("new_state", new_state)
            