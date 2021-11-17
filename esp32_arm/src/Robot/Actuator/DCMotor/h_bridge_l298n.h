#pragma once
#include "DCDriverHBridge.h"



class L298N:public DCDriverHBridge{

    public:
        L298N(){};
        void Init(uint8_t pin_enable, uint8_t pin_a,  uint8_t pin_b);
        void Start(int pwm_speed, bool dir_forward) override;
        void Stop() override;
        void Break() override;

    protected:
        uint8_t _pin_enable;

};
