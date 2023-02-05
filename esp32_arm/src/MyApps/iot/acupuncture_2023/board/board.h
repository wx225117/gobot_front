#pragma once

#include "MyLibs/basic/board_base.h"
#include "MyLibs/board/peripheral/bus_i2c/i2c_iam_master.h"
// #include "MyLibs/board/peripheral/bus_i2c/i2c_slave_node_agent.h"


class AcupunctureBoard_2023: public BoardBase{
    public:
        void Init() override;
        I2C_IamMaster* GetI2C_Master(){return &__i2c_master;};

    private:
        I2C_IamMaster __i2c_master;
        // I2C_SlaveNodeAgent __i2c_slave_node_agent[30];


};