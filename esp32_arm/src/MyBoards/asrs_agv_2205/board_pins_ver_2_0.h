#pragma once

// #ifdef USING_BOARD_AGV_SINGLE_BOARD_VER_2_0

// #define PIN_HCSR04_ECHO 18
// #define PIN_HCSR04_TRIG 19
#define PIN_WS2812B 13


// Common I2C bus is Connected to MCp23018
#define PIN_MAIN_I2C_SDA_2205 27
#define PIN_MAIN_I2C_SCL_2205 26
#define PIN_EXT_I2C_SDA_2205 16
#define PIN_EXT_I2C_SCL_2205 17 

#define PIN_WHEEL_PWM_LEFT_2205 4
#define PIN_WHEEL_DIR_LEFT_2205 2
#define PIN_WHEEL_PWM_RIGHT_2205 25
#define PIN_WHEEL_DIR_RIGHT_2205 33

#define PIN_ALPHA_STEP_2205 22
#define MC23018_PIN_ALPHA_DIR_2205 3
#define MC23018_PIN_ALPHA_ENABLE_2205 2
#define PIN_BETA_STEP_2205 32
#define MC23018_PIN_BETA_DIR_2205 4
#define MC23018_PIN_BETA_ENABLE_2205 5
#define MC23018_PIN_HOME_Z_2205 7
#define MC23018_PIN_HOME_Y_2205 6


#define PIN_RFID_SPI_CLK_2205 18
#define PIN_RFID_SPI_MISO_2205 19
#define PIN_RFID_SPI_MOSI_2205 23
#define PIN_RFID_SS_2205 5
#define PIN_RFID_RESET 11   // double check

#define PIN_BATTERY_VOLTAGE_ADC_2205  34
#define PIN_CHARGER_VOLTAGE_ADC_2205  35
#define PIN_IR_FRONT_2205 36
#define PIN_IR_REAR_2205 39

#define PIN_MCP23018_TEST_2205 13

// #define MCP23018_PIN_EMERGENCY_STOP_BUTTON  11
// #define MCP23018_PIN_INPUT_BUTTON  11

#define WS2812B_COUNT_2205 16

#define I2C_ADDR_MCP23018_2205 0x23
#define I2C_ADDR_VL53L0X_2205 0x29
#define I2C_ADDR_APDS9960_2205 0x39  // Not changable.
#define I2C_ADDR_ADS1115 0x48

#define ADS1115_PIN_UPPER_INNER 0
#define ADS1115_PIN_UPPER_OUTER 1
#define ADS1115_PIN_LOWER_INNER 2
#define ADS1115_PIN_LOWER_OUTER 3
