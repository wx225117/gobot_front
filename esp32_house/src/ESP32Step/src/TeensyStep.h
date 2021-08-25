#pragma once

#include "version.h"

#include "RotateControlBase.h"
#include "StepControlBase.h"

#include "accelerators/LinRotAccelerator.h"
#include "accelerators/LinStepAccelerator.h"
//#include "accelerators/SinRotAccelerator.h"

//#include "timer/generic/TimerField.h"


// TEENSY 3.0 - Teensy 3.6 ==================================================================================

#if defined(__MK20DX128__) || defined(__MK20DX256__) || defined(__MK64FX512__) || defined(__MK66FX1M0__)
#include "timer/teensy3/TimerField2.h"

// TEENSY 4 ================================================================================================

#elif defined(__IMXRT1052__)
#include "timer/teensy4/TimerField.h"

// STM32 ====================================================================================================

#elif defined(__STM32_TBD__)
#include "timer/stm32/TimerField.h"

// ESP32 ======================================================================================

#elif defined(ARDUINO_ARCH_ESP32)
#include "Arduino.h"
#include "timer/esp32/TimerField.h"

// Some other hardware ======================================================================================

#elif defined(__someHardware_TBD__)
#include "timers/someHardware/TimerField2.h"
using StepControlTick = StepControlBase<LinStepAccelerator,TickTimerField>;
using RotateControlTick = RotateControlBase<LinStepAccelerator,TickTimerField>;
#endif

// Linear acceleration -----------------------------------------------------------------------------------------


using MotorControl = MotorControlBase<TimerField>;

using RotateControl = RotateControlBase<LinRotAccelerator, TimerField>;
using StepControl = StepControlBase<LinStepAccelerator, TimerField>;

// Sine acceleration -------------------------------------------------------------------------------------------

// template <unsigned stepPulseWidth = defPW, unsigned accUpdatePeriod = defAP>
// using RotateControlSin = RotateControlBase<SinRotAccelerator, TimerField>;

//template <unsigned p = defPW, unsigned a>
//using StepControlSin = StepControlBase<SinStepAccelerator, p, a>;

// Generic ==========================================================================================

// template <unsigned stepPulseWidth = defPW, unsigned accUpdatePeriod = defAP>
// using RotateControl_tick = RotateControlBase<LinRotAccelerator, stepPulseWidth, accUpdatePeriod>;

// template <unsigned stepPulseWidth = defPW, unsigned a = defAP>
// using StepControl_tick = StepControlBase<LinStepAccelerator, stepPulseWidth, a>;
