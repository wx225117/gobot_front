#include "chessboard_map.h"

// {return " ";};  //TODO: Can return arduino string or pointer?
String ChessboardMap::GetG1Code(ChessboardCell* cell){
    return "G1X0Y0";
}
