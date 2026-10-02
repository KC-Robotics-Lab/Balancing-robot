// I2Cdev library collection - MPU6050 I2C device class
// Based on InvenSense MPU-6050 register map document rev. 2.0, 5/19/2011 (RM-MPU-6000A-00)
// 10/3/2011 by Jeff Rowberg <jeff@rowberg.net>
// Updates should (hopefully) always be available at https://github.com/jrowberg/i2cdevlib
//
// Changelog:
//     ... - ongoing debug release

// NOTE: THIS IS ONLY A PARIAL RELEASE. THIS DEVICE CLASS IS CURRENTLY UNDERGOING ACTIVE
// DEVELOPMENT AND IS STILL MISSING SOME IMPORTANT FEATURES. PLEASE KEEP THIS IN MIND IF
// YOU DECIDE TO USE THIS PARTICULAR CODE FOR ANYTHING.

/* ============================================
I2Cdev device library code is placed under the MIT license
Copyright (c) 2012 Jeff Rowberg

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
===============================================
*/

#ifndef _MPU6050_H_
#define _MPU6050_H_

#include "I2Cdev.h"
#include "helper_3dmath.h"

// supporting link:  http://forum.arduino.cc/index.php?&topic=143444.msg1079517#msg1079517
// also: http://forum.arduino.cc/index.php?&topic=141571.msg1062899#msg1062899s

#ifdef __AVR__
#include <avr/pgmspace.h>
#else
#define PROGMEM /* empty */
#define pgm_read_byte(x) (*(x))
#define pgm_read_word(x) (*(x))
#define pgm_read_float(x) (*(x))
#define PSTR(STR) STR
//#define pgm_read_byte(addr) (*(const unsigned char *)(addr))
//#define pgm_read_word(addr) (*(const unsigned short *)(addr))
//#define pgm_read_dword(addr) (*(const unsigned long *)(addr))
//#define pgm_read_float(addr) (*(const float *)(addr))
#endif

#define MPU6050_INCLUDE_DMP_MOTIONAPPS20


#define MPU6050_ADDRESS_AD0_LOW     0x68 // address pin low (GND), default for InvenSense evaluation board
#define MPU6050_ADDRESS_AD0_HIGH    0x69 // address pin high (VCC)
#define MPU6050_DEFAULT_ADDRESS     MPU6050_ADDRESS_AD0_LOW

#define MPU6050_RA_XG_OFFS_TC       0x00 //[7] PWR_MODE, [6:1] XG_OFFS_TC, [0] OTP_BNK_VLD
#define MPU6050_RA_YG_OFFS_TC       0x01 //[7] PWR_MODE, [6:1] YG_OFFS_TC, [0] OTP_BNK_VLD
#define MPU6050_RA_ZG_OFFS_TC       0x02 //[7] PWR_MODE, [6:1] ZG_OFFS_TC, [0] OTP_BNK_VLD
#define MPU6050_RA_X_FINE_GAIN      0x03 //[7:0] X_FINE_GAIN
#define MPU6050_RA_Y_FINE_GAIN      0x04 //[7:0] Y_FINE_GAIN
#define MPU6050_RA_Z_FINE_GAIN      0x05 //[7:0] Z_FINE_GAIN
#define MPU6050_RA_XA_OFFS_H        0x06 //[15:0] XA_OFFS
#define MPU6050_RA_XA_OFFS_L_TC     0x07
#define MPU6050_RA_YA_OFFS_H        0x08 //[15:0] YA_OFFS
#define MPU6050_RA_YA_OFFS_L_TC     0x09
#define MPU6050_RA_ZA_OFFS_H        0x0A //[15:0] ZA_OFFS
#define MPU6050_RA_ZA_OFFS_L_TC     0x0B
#define MPU6050_RA_SELF_TEST_X      0x0D //[7:5] XA_TEST[4-2], [4:0] XG_TEST[4-0]
#define MPU6050_RA_SELF_TEST_Y      0x0E //[7:5] YA_TEST[4-2], [4:0] YG_TEST[4-0]
#define MPU6050_RA_SELF_TEST_Z      0x0F //[7:5] ZA_TEST[4-2], [4:0] ZG_TEST[4-0]
#define MPU6050_RA_SELF_TEST_A      0x10 //[5:4] XA_TEST[1-0], [3:2] YA_TEST[1-0], [1:0] ZA_TEST[1-0]
#define MPU6050_RA_XG_OFFS_USRH     0x13 //[15:0] XG_OFFS_USR
#define MPU6050_RA_XG_OFFS_USRL     0x14
#define MPU6050_RA_YG_OFFS_USRH     0x15 //[15:0] YG_OFFS_USR
#define MPU6050_RA_YG_OFFS_USRL     0x16
#define MPU6050_RA_ZG_OFFS_USRH     0x17 //[15:0] ZG_OFFS_USR
#define MPU6050_RA_ZG_OFFS_USRL     0x18
#define MPU6050_RA_SMPLRT_DIV       0x19
#define MPU6050_RA_CONFIG           0x1A
#define MPU6050_RA_GYRO_CONFIG      0x1B
#define MPU6050_RA_ACCEL_CONFIG     0x1C
#define MPU6050_RA_FF_THR           0x1D
#define MPU6050_RA_FF_DUR           0x1E
#define MPU6050_RA_MOT_THR          0x1F
#define MPU6050_RA_MOT_DUR          0x20
#define MPU6050_RA_ZRMOT_THR        0x21
#define MPU6050_RA_ZRMOT_DUR        0x22
#define MPU6050_RA_FIFO_EN          0x23
#define MPU6050_RA_I2C_MST_CTRL     0x24
#define MPU6050_RA_I2C_SLV0_ADDR    0x25
#define MPU6050_RA_I2C_SLV0_REG     0x26
#define MPU6050_RA_I2C_SLV0_CTRL    0x27
#define MPU6050_RA_I2C_SLV1_ADDR    0x28
#define MPU6050_RA_I2C_SLV1_REG     0x29
#define MPU6050_RA_I2C_SLV1_CTRL    0x2A
#define MPU6050_RA_I2C_SLV2_ADDR    0x2B
#define MPU6050_RA_I2C_SLV2_REG     0x2C
#define MPU6050_RA_I2C_SLV2_CTRL    0x2D
#define MPU6050_RA_I2C_SLV3_ADDR    0x2E
#define MPU6050_RA_I2C_SLV3_REG     0x2F
#define MPU6050_RA_I2C_SLV3_CTRL    0x30
#define MPU6050_RA_I2C_SLV4_ADDR    0x31
#define MPU6050_RA_I2C_SLV4_REG     0x32
#define MPU6050_RA_I2C_SLV4_DO      0x33
#define MPU6050_RA_I2C_SLV4_CTRL    0x34
#define MPU6050_RA_I2C_SLV4_DI      0x35
#define MPU6050_RA_I2C_MST_STATUS   0x36
#define MPU6050_RA_INT_PIN_CFG      0x37
#define MPU6050_RA_INT_ENABLE       0x38
#define MPU6050_RA_DMP_INT_STATUS   0x39
#define MPU6050_RA_INT_STATUS       0x3A
#define MPU6050_RA_ACCEL_XOUT_H     0x3B
#define MPU6050_RA_ACCEL_XOUT_L     0x3C
#define MPU6050_RA_ACCEL_YOUT_H     0x3D
#define MPU6050_RA_ACCEL_YOUT_L     0x3E
#define MPU6050_RA_ACCEL_ZOUT_H     0x3F
#define MPU6050_RA_ACCEL_ZOUT_L     0x40
#define MPU6050_RA_TEMP_OUT_H       0x41
#define MPU6050_RA_TEMP_OUT_L       0x42
#define MPU6050_RA_GYRO_XOUT_H      0x43
#define MPU6050_RA_GYRO_XOUT_L      0x44
#define MPU6050_RA_GYRO_YOUT_H      0x45
#define MPU6050_RA_GYRO_YOUT_L      0x46
#define MPU6050_RA_GYRO_ZOUT_H      0x47
#define MPU6050_RA_GYRO_ZOUT_L      0x48
#define MPU6050_RA_EXT_SENS_DATA_00 0x49
#define MPU6050_RA_EXT_SENS_DATA_01 0x4A
#define MPU6050_RA_EXT_SENS_DATA_02 0x4B
#define MPU6050_RA_EXT_SENS_DATA_03 0x4C
#define MPU6050_RA_EXT_SENS_DATA_04 0x4D
#define MPU6050_RA_EXT_SENS_DATA_05 0x4E
#define MPU6050_RA_EXT_SENS_DATA_06 0x4F
#define MPU6050_RA_EXT_SENS_DATA_07 0x50
#define MPU6050_RA_EXT_SENS_DATA_08 0x51
#define MPU6050_RA_EXT_SENS_DATA_09 0x52
#define MPU6050_RA_EXT_SENS_DATA_10 0x53
#define MPU6050_RA_EXT_SENS_DATA_11 0x54
#define MPU6050_RA_EXT_SENS_DATA_12 0x55
#define MPU6050_RA_EXT_SENS_DATA_13 0x56
#define MPU6050_RA_EXT_SENS_DATA_14 0x57
#define MPU6050_RA_EXT_SENS_DATA_15 0x58
#define MPU6050_RA_EXT_SENS_DATA_16 0x59
#define MPU6050_RA_EXT_SENS_DATA_17 0x5A
#define MPU6050_RA_EXT_SENS_DATA_18 0x5B
#define MPU6050_RA_EXT_SENS_DATA_19 0x5C
#define MPU6050_RA_EXT_SENS_DATA_20 0x5D
#define MPU6050_RA_EXT_SENS_DATA_21 0x5E
#define MPU6050_RA_EXT_SENS_DATA_22 0x5F
#define MPU6050_RA_EXT_SENS_DATA_23 0x60
#define MPU6050_RA_MOT_DETECT_STATUS    0x61
#define MPU6050_RA_I2C_SLV0_DO      0x63
#define MPU6050_RA_I2C_SLV1_DO      0x64
#define MPU6050_RA_I2C_SLV2_DO      0x65
#define MPU6050_RA_I2C_SLV3_DO      0x66
#define MPU6050_RA_I2C_MST_DELAY_CTRL   0x67
#define MPU6050_RA_SIGNAL_PATH_RESET    0x68
#define MPU6050_RA_MOT_DETECT_CTRL      0x69
#define MPU6050_RA_USER_CTRL        0x6A
#define MPU6050_RA_PWR_MGMT_1       0x6B
#define MPU6050_RA_PWR_MGMT_2       0x6C
#define MPU6050_RA_BANK_SEL         0x6D
#define MPU6050_RA_MEM_START_ADDR   0x6E
#define MPU6050_RA_MEM_R_W          0x6F
#define MPU6050_RA_DMP_CFG_1        0x70
#define MPU6050_RA_DMP_CFG_2        0x71
#define MPU6050_RA_FIFO_COUNTH      0x72
#define MPU6050_RA_FIFO_COUNTL      0x73
#define MPU6050_RA_FIFO_R_W         0x74
#define MPU6050_RA_WHO_AM_I         0x75

#define MPU6050_SELF_TEST_XA_1_BIT     0x07
#define MPU6050_SELF_TEST_XA_1_LENGTH  0x03
#define MPU6050_SELF_TEST_XA_2_BIT     0x05
#define MPU6050_SELF_TEST_XA_2_LENGTH  0x02
#define MPU6050_SELF_TEST_YA_1_BIT     0x07
#define MPU6050_SELF_TEST_YA_1_LENGTH  0x03
#define MPU6050_SELF_TEST_YA_2_BIT     0x03
#define MPU6050_SELF_TEST_YA_2_LENGTH  0x02
#define MPU6050_SELF_TEST_ZA_1_BIT     0x07
#define MPU6050_SELF_TEST_ZA_1_LENGTH  0x03
#define MPU6050_SELF_TEST_ZA_2_BIT     0x01
#define MPU6050_SELF_TEST_ZA_2_LENGTH  0x02

#define MPU6050_SELF_TEST_XG_1_BIT     0x04
#define MPU6050_SELF_TEST_XG_1_LENGTH  0x05
#define MPU6050_SELF_TEST_YG_1_BIT     0x04
#define MPU6050_SELF_TEST_YG_1_LENGTH  0x05
#define MPU6050_SELF_TEST_ZG_1_BIT     0x04
#define MPU6050_SELF_TEST_ZG_1_LENGTH  0x05

#define MPU6050_TC_PWR_MODE_BIT     7
#define MPU6050_TC_OFFSET_BIT       6
#define MPU6050_TC_OFFSET_LENGTH    6
#define MPU6050_TC_OTP_BNK_VLD_BIT  0

#define MPU6050_VDDIO_LEVEL_VLOGIC  0
#define MPU6050_VDDIO_LEVEL_VDD     1

#define MPU6050_CFG_EXT_SYNC_SET_BIT    5
#define MPU6050_CFG_EXT_SYNC_SET_LENGTH 3
#define MPU6050_CFG_DLPF_CFG_BIT    2
#define MPU6050_CFG_DLPF_CFG_LENGTH 3

#define MPU6050_EXT_SYNC_DISABLED       0x0
#define MPU6050_EXT_SYNC_TEMP_OUT_L     0x1
#define MPU6050_EXT_SYNC_GYRO_XOUT_L    0x2
#define MPU6050_EXT_SYNC_GYRO_YOUT_L    0x3
#define MPU6050_EXT_SYNC_GYRO_ZOUT_L    0x4
#define MPU6050_EXT_SYNC_ACCEL_XOUT_L   0x5
#define MPU6050_EXT_SYNC_ACCEL_YOUT_L   0x6
#define MPU6050_EXT_SYNC_ACCEL_ZOUT_L   0x7

#define MPU6050_DLPF_BW_256         0x00
#define MPU6050_DLPF_BW_188         0x01
#define MPU6050_DLPF_BW_98          0x02
#define MPU6050_DLPF_BW_42          0x03
#define MPU6050_DLPF_BW_20          0x04
#define MPU6050_DLPF_BW_10          0x05
#define MPU6050_DLPF_BW_5           0x06

#define MPU6050_GCONFIG_FS_SEL_BIT      4
#define MPU6050_GCONFIG_FS_SEL_LENGTH   2

#define MPU6050_GYRO_FS_250         0x00
#define MPU6050_GYRO_FS_500         0x01
#define MPU6050_GYRO_FS_1000        0x02
#define MPU6050_GYRO_FS_2000        0x03

#define MPU6050_ACONFIG_XA_ST_BIT           7
#define MPU6050_ACONFIG_YA_ST_BIT           6
#define MPU6050_ACONFIG_ZA_ST_BIT           5
#define MPU6050_ACONFIG_AFS_SEL_BIT         4
#define MPU6050_ACONFIG_AFS_SEL_LENGTH      2
#define MPU6050_ACONFIG_ACCEL_HPF_BIT       2
#define MPU6050_ACONFIG_ACCEL_HPF_LENGTH    3

#define MPU6050_ACCEL_FS_2          0x00
#define MPU6050_ACCEL_FS_4          0x01
#define MPU6050_ACCEL_FS_8          0x02
#define MPU6050_ACCEL_FS_16         0x03

#define MPU6050_DHPF_RESET          0x00
#define MPU6050_DHPF_5              0x01
#define MPU6050_DHPF_2P5            0x02
#define MPU6050_DHPF_1P25           0x03
#define MPU6050_DHPF_0P63           0x04
#define MPU6050_DHPF_HOLD           0x07

#define MPU6050_TEMP_FIFO_EN_BIT    7
#define MPU6050_XG_FIFO_EN_BIT      6
#define MPU6050_YG_FIFO_EN_BIT      5
#define MPU6050_ZG_FIFO_EN_BIT      4
#define MPU6050_ACCEL_FIFO_EN_BIT   3
#define MPU6050_SLV2_FIFO_EN_BIT    2
#define MPU6050_SLV1_FIFO_EN_BIT    1
#define MPU6050_SLV0_FIFO_EN_BIT    0

#define MPU6050_MULT_MST_EN_BIT     7
#define MPU6050_WAIT_FOR_ES_BIT     6
#define MPU6050_SLV_3_FIFO_EN_BIT   5
#define MPU6050_I2C_MST_P_NSR_BIT   4
#define MPU6050_I2C_MST_CLK_BIT     3
#define MPU6050_I2C_MST_CLK_LENGTH  4

#define MPU6050_CLOCK_DIV_348       0x0
#define MPU6050_CLOCK_DIV_333       0x1
#define MPU6050_CLOCK_DIV_320       0x2
#define MPU6050_CLOCK_DIV_308       0x3
#define MPU6050_CLOCK_DIV_296       0x4
#define MPU6050_CLOCK_DIV_286       0x5
#define MPU6050_CLOCK_DIV_276       0x6
#define MPU6050_CLOCK_DIV_267       0x7
#define MPU6050_CLOCK_DIV_258       0x8
#define MPU6050_CLOCK_DIV_500       0x9
#define MPU6050_CLOCK_DIV_471       0xA
#define MPU6050_CLOCK_DIV_444       0xB
#define MPU6050_CLOCK_DIV_421       0xC
#define MPU6050_CLOCK_DIV_400       0xD
#define MPU6050_CLOCK_DIV_381       0xE
#define MPU6050_CLOCK_DIV_364       0xF

#define MPU6050_I2C_SLV_RW_BIT      7
#define MPU6050_I2C_SLV_ADDR_BIT    6
#define MPU6050_I2C_SLV_ADDR_LENGTH 7
#define MPU6050_I2C_SLV_EN_BIT      7
#define MPU6050_I2C_SLV_BYTE_SW_BIT 6
#define MPU6050_I2C_SLV_REG_DIS_BIT 5
#define MPU6050_I2C_SLV_GRP_BIT     4
#define MPU6050_I2C_SLV_LEN_BIT     3
#define MPU6050_I2C_SLV_LEN_LENGTH  4

#define MPU6050_I2C_SLV4_RW_BIT         7
#define MPU6050_I2C_SLV4_ADDR_BIT       6
#define MPU6050_I2C_SLV4_ADDR_LENGTH    7
#define MPU6050_I2C_SLV4_EN_BIT         7
#define MPU6050_I2C_SLV4_INT_EN_BIT     6
#define MPU6050_I2C_SLV4_REG_DIS_BIT    5
#define MPU6050_I2C_SLV4_MST_DLY_BIT    4
#define MPU6050_I2C_SLV4_MST_DLY_LENGTH 5

#define MPU6050_MST_PASS_THROUGH_BIT    7
#define MPU6050_MST_I2C_SLV4_DONE_BIT   6
#define MPU6050_MST_I2C_LOST_ARB_BIT    5
#define MPU6050_MST_I2C_SLV4_NACK_BIT   4
#define MPU6050_MST_I2C_SLV3_NACK_BIT   3
#define MPU6050_MST_I2C_SLV2_NACK_BIT   2
#define MPU6050_MST_I2C_SLV1_NACK_BIT   1
#define MPU6050_MST_I2C_SLV0_NACK_BIT   0

#define MPU6050_INTCFG_INT_LEVEL_BIT        7
#define MPU6050_INTCFG_INT_OPEN_BIT         6
#define MPU6050_INTCFG_LATCH_INT_EN_BIT     5
#define MPU6050_INTCFG_INT_RD_CLEAR_BIT     4
#define MPU6050_INTCFG_FSYNC_INT_LEVEL_BIT  3
#define MPU6050_INTCFG_FSYNC_INT_EN_BIT     2
#define MPU6050_INTCFG_I2C_BYPASS_EN_BIT    1
#define MPU6050_INTCFG_CLKOUT_EN_BIT        0

#define MPU6050_INTMODE_ACTIVEHIGH  0x00
#define MPU6050_INTMODE_ACTIVELOW   0x01

#define MPU6050_INTDRV_PUSHPULL     0x00
#define MPU6050_INTDRV_OPENDRAIN    0x01

#define MPU6050_INTLATCH_50USPULSE  0x00
#define MPU6050_INTLATCH_WAITCLEAR  0x01

#define MPU6050_INTCLEAR_STATUSREAD 0x00
#define MPU6050_INTCLEAR_ANYREAD    0x01

#define MPU6050_INTERRUPT_FF_BIT            7
#define MPU6050_INTERRUPT_MOT_BIT           6
#define MPU6050_INTERRUPT_ZMOT_BIT          5
#define MPU6050_INTERRUPT_FIFO_OFLOW_BIT    4
#define MPU6050_INTERRUPT_I2C_MST_INT_BIT   3
#define MPU6050_INTERRUPT_PLL_RDY_INT_BIT   2
#define MPU6050_INTERRUPT_DMP_INT_BIT       1
#define MPU6050_INTERRUPT_DATA_RDY_BIT      0

// TODO: figure out what these actually do
// UMPL source code is not very obivous
#define MPU6050_DMPINT_5_BIT            5
#define MPU6050_DMPINT_4_BIT            4
#define MPU6050_DMPINT_3_BIT            3
#define MPU6050_DMPINT_2_BIT            2
#define MPU6050_DMPINT_1_BIT            1
#define MPU6050_DMPINT_0_BIT            0

#define MPU6050_MOTION_MOT_XNEG_BIT     7
#define MPU6050_MOTION_MOT_XPOS_BIT     6
#define MPU6050_MOTION_MOT_YNEG_BIT     5
#define MPU6050_MOTION_MOT_YPOS_BIT     4
#define MPU6050_MOTION_MOT_ZNEG_BIT     3
#define MPU6050_MOTION_MOT_ZPOS_BIT     2
#define MPU6050_MOTION_MOT_ZRMOT_BIT    0

#define MPU6050_DELAYCTRL_DELAY_ES_SHADOW_BIT   7
#define MPU6050_DELAYCTRL_I2C_SLV4_DLY_EN_BIT   4
#define MPU6050_DELAYCTRL_I2C_SLV3_DLY_EN_BIT   3
#define MPU6050_DELAYCTRL_I2C_SLV2_DLY_EN_BIT   2
#define MPU6050_DELAYCTRL_I2C_SLV1_DLY_EN_BIT   1
#define MPU6050_DELAYCTRL_I2C_SLV0_DLY_EN_BIT   0

#define MPU6050_PATHRESET_GYRO_RESET_BIT    2
#define MPU6050_PATHRESET_ACCEL_RESET_BIT   1
#define MPU6050_PATHRESET_TEMP_RESET_BIT    0

#define MPU6050_DETECT_ACCEL_ON_DELAY_BIT       5
#define MPU6050_DETECT_ACCEL_ON_DELAY_LENGTH    2
#define MPU6050_DETECT_FF_COUNT_BIT             3
#define MPU6050_DETECT_FF_COUNT_LENGTH          2
#define MPU6050_DETECT_MOT_COUNT_BIT            1
#define MPU6050_DETECT_MOT_COUNT_LENGTH         2

#define MPU6050_DETECT_DECREMENT_RESET  0x0
#define MPU6050_DETECT_DECREMENT_1      0x1
#define MPU6050_DETECT_DECREMENT_2      0x2
#define MPU6050_DETECT_DECREMENT_4      0x3

#define MPU6050_USERCTRL_DMP_EN_BIT             7
#define MPU6050_USERCTRL_FIFO_EN_BIT            6
#define MPU6050_USERCTRL_I2C_MST_EN_BIT         5
#define MPU6050_USERCTRL_I2C_IF_DIS_BIT         4
#define MPU6050_USERCTRL_DMP_RESET_BIT          3
#define MPU6050_USERCTRL_FIFO_RESET_BIT         2
#define MPU6050_USERCTRL_I2C_MST_RESET_BIT      1
#define MPU6050_USERCTRL_SIG_COND_RESET_BIT     0

#define MPU6050_PWR1_DEVICE_RESET_BIT   7
#define MPU6050_PWR1_SLEEP_BIT          6
#define MPU6050_PWR1_CYCLE_BIT          5
#define MPU6050_PWR1_TEMP_DIS_BIT       3
#define MPU6050_PWR1_CLKSEL_BIT         2
#define MPU6050_PWR1_CLKSEL_LENGTH      3

#define MPU6050_CLOCK_INTERNAL          0x00
#define MPU6050_CLOCK_PLL_XGYRO         0x01
#define MPU6050_CLOCK_PLL_YGYRO         0x02
#define MPU6050_CLOCK_PLL_ZGYRO         0x03
#define MPU6050_CLOCK_PLL_EXT32K        0x04
#define MPU6050_CLOCK_PLL_EXT19M        0x05
#define MPU6050_CLOCK_KEEP_RESET        0x07

#define MPU6050_PWR2_LP_WAKE_CTRL_BIT       7
#define MPU6050_PWR2_LP_WAKE_CTRL_LENGTH    2
#define MPU6050_PWR2_STBY_XA_BIT            5
#define MPU6050_PWR2_STBY_YA_BIT            4
#define MPU6050_PWR2_STBY_ZA_BIT            3
#define MPU6050_PWR2_STBY_XG_BIT            2
#define MPU6050_PWR2_STBY_YG_BIT            1
#define MPU6050_PWR2_STBY_ZG_BIT            0

#define MPU6050_WAKE_FREQ_1P25      0x0
#define MPU6050_WAKE_FREQ_2P5       0x1
#define MPU6050_WAKE_FREQ_5         0x2
#define MPU6050_WAKE_FREQ_10        0x3

#define MPU6050_BANKSEL_PRFTCH_EN_BIT       6
#define MPU6050_BANKSEL_CFG_USER_BANK_BIT   5
#define MPU6050_BANKSEL_MEM_SEL_BIT         4
#define MPU6050_BANKSEL_MEM_SEL_LENGTH      5

#define MPU6050_WHO_AM_I_BIT        6
#define MPU6050_WHO_AM_I_LENGTH     6

#define MPU6050_DMP_MEMORY_BANKS        8
#define MPU6050_DMP_MEMORY_BANK_SIZE    256
#define MPU6050_DMP_MEMORY_CHUNK_SIZE   16



// note: DMP code memory blocks defined at end of header file

class MPU6050 {
    public:
        MPU6050(uint8_t address=MPU6050_DEFAULT_ADDRESS);

        void initialize(I2C_HandleTypeDef *i2c);
        bool testConnection(I2C_HandleTypeDef *i2c);

        // AUX_VDDIO register
        uint8_t getAuxVDDIOLevel(I2C_HandleTypeDef *i2c);
        void setAuxVDDIOLevel(I2C_HandleTypeDef *i2c, uint8_t level);

        // SMPLRT_DIV register
        uint8_t getRate(I2C_HandleTypeDef *i2c);
        void setRate(I2C_HandleTypeDef *i2c, uint8_t rate);

        // CONFIG register
        uint8_t getExternalFrameSync(I2C_HandleTypeDef *i2c);
        void setExternalFrameSync(I2C_HandleTypeDef *i2c, uint8_t sync);
        uint8_t getDLPFMode(I2C_HandleTypeDef *i2c);
        void setDLPFMode(I2C_HandleTypeDef *i2c, uint8_t bandwidth);

        // GYRO_CONFIG register
        uint8_t getFullScaleGyroRange(I2C_HandleTypeDef *i2c);
        void setFullScaleGyroRange(I2C_HandleTypeDef *i2c, uint8_t range);

        // SELF_TEST registers
        uint8_t getAccelXSelfTestFactoryTrim(I2C_HandleTypeDef *i2c);
        uint8_t getAccelYSelfTestFactoryTrim(I2C_HandleTypeDef *i2c);
        uint8_t getAccelZSelfTestFactoryTrim(I2C_HandleTypeDef *i2c);

        uint8_t getGyroXSelfTestFactoryTrim(I2C_HandleTypeDef *i2c);
        uint8_t getGyroYSelfTestFactoryTrim(I2C_HandleTypeDef *i2c);
        uint8_t getGyroZSelfTestFactoryTrim(I2C_HandleTypeDef *i2c);

        // ACCEL_CONFIG register
        bool getAccelXSelfTest(I2C_HandleTypeDef *i2c);
        void setAccelXSelfTest(I2C_HandleTypeDef *i2c, bool enabled);
        bool getAccelYSelfTest(I2C_HandleTypeDef *i2c);
        void setAccelYSelfTest(I2C_HandleTypeDef *i2c, bool enabled);
        bool getAccelZSelfTest(I2C_HandleTypeDef *i2c);
        void setAccelZSelfTest(I2C_HandleTypeDef *i2c, bool enabled);
        uint8_t getFullScaleAccelRange(I2C_HandleTypeDef *i2c);
        void setFullScaleAccelRange(I2C_HandleTypeDef *i2c, uint8_t range);
        uint8_t getDHPFMode(I2C_HandleTypeDef *i2c);
        void setDHPFMode(I2C_HandleTypeDef *i2c, uint8_t mode);

        // FF_THR register
        uint8_t getFreefallDetectionThreshold(I2C_HandleTypeDef *i2c);
        void setFreefallDetectionThreshold(I2C_HandleTypeDef *i2c, uint8_t threshold);

        // FF_DUR register
        uint8_t getFreefallDetectionDuration(I2C_HandleTypeDef *i2c);
        void setFreefallDetectionDuration(I2C_HandleTypeDef *i2c, uint8_t duration);

        // MOT_THR register
        uint8_t getMotionDetectionThreshold(I2C_HandleTypeDef *i2c);
        void setMotionDetectionThreshold(I2C_HandleTypeDef *i2c, uint8_t threshold);

        // MOT_DUR register
        uint8_t getMotionDetectionDuration(I2C_HandleTypeDef *i2c);
        void setMotionDetectionDuration(I2C_HandleTypeDef *i2c, uint8_t duration);

        // ZRMOT_THR register
        uint8_t getZeroMotionDetectionThreshold(I2C_HandleTypeDef *i2c);
        void setZeroMotionDetectionThreshold(I2C_HandleTypeDef *i2c, uint8_t threshold);

        // ZRMOT_DUR register
        uint8_t getZeroMotionDetectionDuration(I2C_HandleTypeDef *i2c);
        void setZeroMotionDetectionDuration(I2C_HandleTypeDef *i2c, uint8_t duration);

        // FIFO_EN register
        bool getTempFIFOEnabled(I2C_HandleTypeDef *i2c);
        void setTempFIFOEnabled(I2C_HandleTypeDef *i2c, bool enabled);
        bool getXGyroFIFOEnabled(I2C_HandleTypeDef *i2c);
        void setXGyroFIFOEnabled(I2C_HandleTypeDef *i2c, bool enabled);
        bool getYGyroFIFOEnabled(I2C_HandleTypeDef *i2c);
        void setYGyroFIFOEnabled(I2C_HandleTypeDef *i2c, bool enabled);
        bool getZGyroFIFOEnabled(I2C_HandleTypeDef *i2c);
        void setZGyroFIFOEnabled(I2C_HandleTypeDef *i2c, bool enabled);
        bool getAccelFIFOEnabled(I2C_HandleTypeDef *i2c);
        void setAccelFIFOEnabled(I2C_HandleTypeDef *i2c, bool enabled);
        bool getSlave2FIFOEnabled(I2C_HandleTypeDef *i2c);
        void setSlave2FIFOEnabled(I2C_HandleTypeDef *i2c, bool enabled);
        bool getSlave1FIFOEnabled(I2C_HandleTypeDef *i2c);
        void setSlave1FIFOEnabled(I2C_HandleTypeDef *i2c, bool enabled);
        bool getSlave0FIFOEnabled(I2C_HandleTypeDef *i2c);
        void setSlave0FIFOEnabled(I2C_HandleTypeDef *i2c, bool enabled);

        // I2C_MST_CTRL register
        bool getMultiMasterEnabled(I2C_HandleTypeDef *i2c);
        void setMultiMasterEnabled(I2C_HandleTypeDef *i2c, bool enabled);
        bool getWaitForExternalSensorEnabled(I2C_HandleTypeDef *i2c);
        void setWaitForExternalSensorEnabled(I2C_HandleTypeDef *i2c, bool enabled);
        bool getSlave3FIFOEnabled(I2C_HandleTypeDef *i2c);
        void setSlave3FIFOEnabled(I2C_HandleTypeDef *i2c, bool enabled);
        bool getSlaveReadWriteTransitionEnabled(I2C_HandleTypeDef *i2c);
        void setSlaveReadWriteTransitionEnabled(I2C_HandleTypeDef *i2c, bool enabled);
        uint8_t getMasterClockSpeed(I2C_HandleTypeDef *i2c);
        void setMasterClockSpeed(I2C_HandleTypeDef *i2c, uint8_t speed);

        // I2C_SLV* registers (Slave 0-3)
        uint8_t getSlaveAddress(I2C_HandleTypeDef *i2c, uint8_t num);
        void setSlaveAddress(I2C_HandleTypeDef *i2c, uint8_t num, uint8_t address);
        uint8_t getSlaveRegister(I2C_HandleTypeDef *i2c, uint8_t num);
        void setSlaveRegister(I2C_HandleTypeDef *i2c, uint8_t num, uint8_t reg);
        bool getSlaveEnabled(I2C_HandleTypeDef *i2c, uint8_t num);
        void setSlaveEnabled(I2C_HandleTypeDef *i2c, uint8_t num, bool enabled);
        bool getSlaveWordByteSwap(I2C_HandleTypeDef *i2c, uint8_t num);
        void setSlaveWordByteSwap(I2C_HandleTypeDef *i2c, uint8_t num, bool enabled);
        bool getSlaveWriteMode(I2C_HandleTypeDef *i2c, uint8_t num);
        void setSlaveWriteMode(I2C_HandleTypeDef *i2c, uint8_t num, bool mode);
        bool getSlaveWordGroupOffset(I2C_HandleTypeDef *i2c, uint8_t num);
        void setSlaveWordGroupOffset(I2C_HandleTypeDef *i2c, uint8_t num, bool enabled);
        uint8_t getSlaveDataLength(I2C_HandleTypeDef *i2c, uint8_t num);
        void setSlaveDataLength(I2C_HandleTypeDef *i2c, uint8_t num, uint8_t length);

        // I2C_SLV* registers (Slave 4)
        uint8_t getSlave4Address(I2C_HandleTypeDef *i2c);
        void setSlave4Address(I2C_HandleTypeDef *i2c, uint8_t address);
        uint8_t getSlave4Register(I2C_HandleTypeDef *i2c);
        void setSlave4Register(I2C_HandleTypeDef *i2c, uint8_t reg);
        void setSlave4OutputByte(I2C_HandleTypeDef *i2c, uint8_t data);
        bool getSlave4Enabled(I2C_HandleTypeDef *i2c);
        void setSlave4Enabled(I2C_HandleTypeDef *i2c, bool enabled);
        bool getSlave4InterruptEnabled(I2C_HandleTypeDef *i2c);
        void setSlave4InterruptEnabled(I2C_HandleTypeDef *i2c, bool enabled);
        bool getSlave4WriteMode(I2C_HandleTypeDef *i2c);
        void setSlave4WriteMode(I2C_HandleTypeDef *i2c, bool mode);
        uint8_t getSlave4MasterDelay(I2C_HandleTypeDef *i2c);
        void setSlave4MasterDelay(I2C_HandleTypeDef *i2c, uint8_t delay);
        uint8_t getSlate4InputByte(I2C_HandleTypeDef *i2c);

        // I2C_MST_STATUS register
        bool getPassthroughStatus(I2C_HandleTypeDef *i2c);
        bool getSlave4IsDone(I2C_HandleTypeDef *i2c);
        bool getLostArbitration(I2C_HandleTypeDef *i2c);
        bool getSlave4Nack(I2C_HandleTypeDef *i2c);
        bool getSlave3Nack(I2C_HandleTypeDef *i2c);
        bool getSlave2Nack(I2C_HandleTypeDef *i2c);
        bool getSlave1Nack(I2C_HandleTypeDef *i2c);
        bool getSlave0Nack(I2C_HandleTypeDef *i2c);

        // INT_PIN_CFG register
        bool getInterruptMode(I2C_HandleTypeDef *i2c);
        void setInterruptMode(I2C_HandleTypeDef *i2c, bool mode);
        bool getInterruptDrive(I2C_HandleTypeDef *i2c);
        void setInterruptDrive(I2C_HandleTypeDef *i2c, bool drive);
        bool getInterruptLatch(I2C_HandleTypeDef *i2c);
        void setInterruptLatch(I2C_HandleTypeDef *i2c, bool latch);
        bool getInterruptLatchClear(I2C_HandleTypeDef *i2c);
        void setInterruptLatchClear(I2C_HandleTypeDef *i2c, bool clear);
        bool getFSyncInterruptLevel(I2C_HandleTypeDef *i2c);
        void setFSyncInterruptLevel(I2C_HandleTypeDef *i2c, bool level);
        bool getFSyncInterruptEnabled(I2C_HandleTypeDef *i2c);
        void setFSyncInterruptEnabled(I2C_HandleTypeDef *i2c, bool enabled);
        bool getI2CBypassEnabled(I2C_HandleTypeDef *i2c);
        void setI2CBypassEnabled(I2C_HandleTypeDef *i2c, bool enabled);
        bool getClockOutputEnabled(I2C_HandleTypeDef *i2c);
        void setClockOutputEnabled(I2C_HandleTypeDef *i2c, bool enabled);

        // INT_ENABLE register
        uint8_t getIntEnabled(I2C_HandleTypeDef *i2c);
        void setIntEnabled(I2C_HandleTypeDef *i2c, uint8_t enabled);
        bool getIntFreefallEnabled(I2C_HandleTypeDef *i2c);
        void setIntFreefallEnabled(I2C_HandleTypeDef *i2c, bool enabled);
        bool getIntMotionEnabled(I2C_HandleTypeDef *i2c);
        void setIntMotionEnabled(I2C_HandleTypeDef *i2c, bool enabled);
        bool getIntZeroMotionEnabled(I2C_HandleTypeDef *i2c);
        void setIntZeroMotionEnabled(I2C_HandleTypeDef *i2c, bool enabled);
        bool getIntFIFOBufferOverflowEnabled(I2C_HandleTypeDef *i2c);
        void setIntFIFOBufferOverflowEnabled(I2C_HandleTypeDef *i2c, bool enabled);
        bool getIntI2CMasterEnabled(I2C_HandleTypeDef *i2c);
        void setIntI2CMasterEnabled(I2C_HandleTypeDef *i2c, bool enabled);
        bool getIntDataReadyEnabled(I2C_HandleTypeDef *i2c);
        void setIntDataReadyEnabled(I2C_HandleTypeDef *i2c, bool enabled);

        // INT_STATUS register
        uint8_t getIntStatus(I2C_HandleTypeDef *i2c);
        bool getIntFreefallStatus(I2C_HandleTypeDef *i2c);
        bool getIntMotionStatus(I2C_HandleTypeDef *i2c);
        bool getIntZeroMotionStatus(I2C_HandleTypeDef *i2c);
        bool getIntFIFOBufferOverflowStatus(I2C_HandleTypeDef *i2c);
        bool getIntI2CMasterStatus(I2C_HandleTypeDef *i2c);
        bool getIntDataReadyStatus(I2C_HandleTypeDef *i2c);

        // ACCEL_*OUT_* registers
        void getMotion9(I2C_HandleTypeDef *i2c, int16_t* ax, int16_t* ay, int16_t* az, int16_t* gx, int16_t* gy, int16_t* gz, int16_t* mx, int16_t* my, int16_t* mz);
        void getMotion6(I2C_HandleTypeDef *i2c, int16_t* ax, int16_t* ay, int16_t* az, int16_t* gx, int16_t* gy, int16_t* gz);
        void getAcceleration(I2C_HandleTypeDef *i2c, int16_t* x, int16_t* y, int16_t* z);
        int16_t getAccelerationX(I2C_HandleTypeDef *i2c);
        int16_t getAccelerationY(I2C_HandleTypeDef *i2c);
        int16_t getAccelerationZ(I2C_HandleTypeDef *i2c);

        // TEMP_OUT_* registers
        int16_t getTemperature(I2C_HandleTypeDef *i2c);

        // GYRO_*OUT_* registers
        void getRotation(I2C_HandleTypeDef *i2c, int16_t* x, int16_t* y, int16_t* z);
        int16_t getRotationX(I2C_HandleTypeDef *i2c);
        int16_t getRotationY(I2C_HandleTypeDef *i2c);
        int16_t getRotationZ(I2C_HandleTypeDef *i2c);

        // EXT_SENS_DATA_* registers
        uint8_t getExternalSensorByte(I2C_HandleTypeDef *i2c, int position);
        uint16_t getExternalSensorWord(I2C_HandleTypeDef *i2c, int position);
        uint32_t getExternalSensorDWord(I2C_HandleTypeDef *i2c, int position);

        // MOT_DETECT_STATUS register
        uint8_t getMotionStatus(I2C_HandleTypeDef *i2c);
        bool getXNegMotionDetected(I2C_HandleTypeDef *i2c);
        bool getXPosMotionDetected(I2C_HandleTypeDef *i2c);
        bool getYNegMotionDetected(I2C_HandleTypeDef *i2c);
        bool getYPosMotionDetected(I2C_HandleTypeDef *i2c);
        bool getZNegMotionDetected(I2C_HandleTypeDef *i2c);
        bool getZPosMotionDetected(I2C_HandleTypeDef *i2c);
        bool getZeroMotionDetected(I2C_HandleTypeDef *i2c);

        // I2C_SLV*_DO register
        void setSlaveOutputByte(I2C_HandleTypeDef *i2c, uint8_t num, uint8_t data);

        // I2C_MST_DELAY_CTRL register
        bool getExternalShadowDelayEnabled(I2C_HandleTypeDef *i2c);
        void setExternalShadowDelayEnabled(I2C_HandleTypeDef *i2c, bool enabled);
        bool getSlaveDelayEnabled(I2C_HandleTypeDef *i2c, uint8_t num);
        void setSlaveDelayEnabled(I2C_HandleTypeDef *i2c, uint8_t num, bool enabled);

        // SIGNAL_PATH_RESET register
        void resetGyroscopePath(I2C_HandleTypeDef *i2c);
        void resetAccelerometerPath(I2C_HandleTypeDef *i2c);
        void resetTemperaturePath(I2C_HandleTypeDef *i2c);

        // MOT_DETECT_CTRL register
        uint8_t getAccelerometerPowerOnDelay(I2C_HandleTypeDef *i2c);
        void setAccelerometerPowerOnDelay(I2C_HandleTypeDef *i2c, uint8_t delay);
        uint8_t getFreefallDetectionCounterDecrement(I2C_HandleTypeDef *i2c);
        void setFreefallDetectionCounterDecrement(I2C_HandleTypeDef *i2c, uint8_t decrement);
        uint8_t getMotionDetectionCounterDecrement(I2C_HandleTypeDef *i2c);
        void setMotionDetectionCounterDecrement(I2C_HandleTypeDef *i2c, uint8_t decrement);

        // USER_CTRL register
        bool getFIFOEnabled(I2C_HandleTypeDef *i2c);
        void setFIFOEnabled(I2C_HandleTypeDef *i2c, bool enabled);
        bool getI2CMasterModeEnabled(I2C_HandleTypeDef *i2c);
        void setI2CMasterModeEnabled(I2C_HandleTypeDef *i2c, bool enabled);
        void switchSPIEnabled(I2C_HandleTypeDef *i2c, bool enabled);
        void resetFIFO(I2C_HandleTypeDef *i2c);
        void resetI2CMaster(I2C_HandleTypeDef *i2c);
        void resetSensors(I2C_HandleTypeDef *i2c);

        // PWR_MGMT_1 register
        void reset(I2C_HandleTypeDef *i2c);
        bool getSleepEnabled(I2C_HandleTypeDef *i2c);
        void setSleepEnabled(I2C_HandleTypeDef *i2c,bool enabled);
        bool getWakeCycleEnabled(I2C_HandleTypeDef *i2c);
        void setWakeCycleEnabled(I2C_HandleTypeDef *i2c,bool enabled);
        bool getTempSensorEnabled(I2C_HandleTypeDef *i2c);
        void setTempSensorEnabled(I2C_HandleTypeDef *i2c, bool enabled);
        uint8_t getClockSource(I2C_HandleTypeDef *i2c);
        void setClockSource(I2C_HandleTypeDef *i2c,uint8_t source);

        // PWR_MGMT_2 register
        uint8_t getWakeFrequency(I2C_HandleTypeDef *i2c);
        void setWakeFrequency(I2C_HandleTypeDef *i2c, uint8_t frequency);
        bool getStandbyXAccelEnabled(I2C_HandleTypeDef *i2c);
        void setStandbyXAccelEnabled(I2C_HandleTypeDef *i2c,bool enabled);
        bool getStandbyYAccelEnabled(I2C_HandleTypeDef *i2c);
        void setStandbyYAccelEnabled(I2C_HandleTypeDef *i2c, bool enabled);
        bool getStandbyZAccelEnabled(I2C_HandleTypeDef *i2c);
        void setStandbyZAccelEnabled(I2C_HandleTypeDef *i2c,bool enabled);
        bool getStandbyXGyroEnabled(I2C_HandleTypeDef *i2c);
        void setStandbyXGyroEnabled(I2C_HandleTypeDef *i2c, bool enabled);
        bool getStandbyYGyroEnabled(I2C_HandleTypeDef *i2c);
        void setStandbyYGyroEnabled(I2C_HandleTypeDef *i2c, bool enabled);
        bool getStandbyZGyroEnabled(I2C_HandleTypeDef *i2c);
        void setStandbyZGyroEnabled(I2C_HandleTypeDef *i2c,bool enabled);

        // FIFO_COUNT_* registers
        uint16_t getFIFOCount(I2C_HandleTypeDef *i2c);

        // FIFO_R_W register
        uint8_t getFIFOByte(I2C_HandleTypeDef *i2c);
		int8_t GetCurrentFIFOPacket(I2C_HandleTypeDef *i2c, uint8_t *data, uint8_t length);
        void setFIFOByte(I2C_HandleTypeDef *i2c, uint8_t data);
        void getFIFOBytes(I2C_HandleTypeDef *i2c, uint8_t *data, uint8_t length);

        // WHO_AM_I register
        uint8_t getDeviceID(I2C_HandleTypeDef *i2c);
        void setDeviceID(I2C_HandleTypeDef *i2c, uint8_t id);
        
        // ======== UNDOCUMENTED/DMP REGISTERS/METHODS ========
        
        // XG_OFFS_TC register
        uint8_t getOTPBankValid(I2C_HandleTypeDef *i2c);
        void setOTPBankValid(I2C_HandleTypeDef *i2c, bool enabled);
        int8_t getXGyroOffsetTC(I2C_HandleTypeDef *i2c);
        void setXGyroOffsetTC(I2C_HandleTypeDef *i2c, int8_t offset);

        // YG_OFFS_TC register
        int8_t getYGyroOffsetTC(I2C_HandleTypeDef *i2c);
        void setYGyroOffsetTC(I2C_HandleTypeDef *i2c, int8_t offset);

        // ZG_OFFS_TC register
        int8_t getZGyroOffsetTC(I2C_HandleTypeDef *i2c);
        void setZGyroOffsetTC(I2C_HandleTypeDef *i2c, int8_t offset);

        // X_FINE_GAIN register
        int8_t getXFineGain(I2C_HandleTypeDef *i2c);
        void setXFineGain(I2C_HandleTypeDef *i2c, int8_t gain);

        // Y_FINE_GAIN register
        int8_t getYFineGain(I2C_HandleTypeDef *i2c);
        void setYFineGain(I2C_HandleTypeDef *i2c, int8_t gain);

        // Z_FINE_GAIN register
        int8_t getZFineGain(I2C_HandleTypeDef *i2c);
        void setZFineGain(I2C_HandleTypeDef *i2c, int8_t gain);

        // XA_OFFS_* registers
        int16_t getXAccelOffset(I2C_HandleTypeDef *i2c);
        void setXAccelOffset(I2C_HandleTypeDef *i2c, int16_t offset);

        // YA_OFFS_* register
        int16_t getYAccelOffset(I2C_HandleTypeDef *i2c);
        void setYAccelOffset(I2C_HandleTypeDef *i2c, int16_t offset);

        // ZA_OFFS_* register
        int16_t getZAccelOffset(I2C_HandleTypeDef *i2c);
        void setZAccelOffset(I2C_HandleTypeDef *i2c, int16_t offset);

        // XG_OFFS_USR* registers
        int16_t getXGyroOffset(I2C_HandleTypeDef *i2c);
        void setXGyroOffset(I2C_HandleTypeDef *i2c, int16_t offset);

        // YG_OFFS_USR* register
        int16_t getYGyroOffset(I2C_HandleTypeDef *i2c);
        void setYGyroOffset(I2C_HandleTypeDef *i2c, int16_t offset);

        // ZG_OFFS_USR* register
        int16_t getZGyroOffset(I2C_HandleTypeDef *i2c);
        void setZGyroOffset(I2C_HandleTypeDef *i2c, int16_t offset);
        
        // INT_ENABLE register (DMP functions)
        bool getIntPLLReadyEnabled(I2C_HandleTypeDef *i2c);
        void setIntPLLReadyEnabled(I2C_HandleTypeDef *i2c, bool enabled);
        bool getIntDMPEnabled(I2C_HandleTypeDef *i2c);
        void setIntDMPEnabled(I2C_HandleTypeDef *i2c, bool enabled);
        
        // DMP_INT_STATUS
        bool getDMPInt5Status(I2C_HandleTypeDef *i2c);
        bool getDMPInt4Status(I2C_HandleTypeDef *i2c);
        bool getDMPInt3Status(I2C_HandleTypeDef *i2c);
        bool getDMPInt2Status(I2C_HandleTypeDef *i2c);
        bool getDMPInt1Status(I2C_HandleTypeDef *i2c);
        bool getDMPInt0Status(I2C_HandleTypeDef *i2c);

        // INT_STATUS register (DMP functions)
        bool getIntPLLReadyStatus(I2C_HandleTypeDef *i2c);
        bool getIntDMPStatus(I2C_HandleTypeDef *i2c);
        
        // USER_CTRL register (DMP functions)
        bool getDMPEnabled(I2C_HandleTypeDef *i2c);
        void setDMPEnabled(I2C_HandleTypeDef *i2c, bool enabled);
        void resetDMP(I2C_HandleTypeDef *i2c);
        
        // BANK_SEL register
        void setMemoryBank(I2C_HandleTypeDef *i2c, uint8_t bank, bool prefetchEnabled=false, bool userBank=false);
        
        // MEM_START_ADDR register
        void setMemoryStartAddress(I2C_HandleTypeDef *i2c, uint8_t address);
        
        // MEM_R_W register
        uint8_t readMemoryByte(I2C_HandleTypeDef *i2c);
        void writeMemoryByte(I2C_HandleTypeDef *i2c, uint8_t data);
        void readMemoryBlock(I2C_HandleTypeDef *i2c, uint8_t *data, uint16_t dataSize, uint8_t bank=0, uint8_t address=0);
        bool writeMemoryBlock(I2C_HandleTypeDef *i2c, const uint8_t *data, uint16_t dataSize, uint8_t bank=0, uint8_t address=0, bool verify=true, bool useProgMem=false);
        bool writeProgMemoryBlock(I2C_HandleTypeDef *i2c, const uint8_t *data, uint16_t dataSize, uint8_t bank=0, uint8_t address=0, bool verify=true);

        bool writeDMPConfigurationSet(I2C_HandleTypeDef *i2c, const uint8_t *data, uint16_t dataSize, bool useProgMem=false);
        bool writeProgDMPConfigurationSet(I2C_HandleTypeDef *i2c, const uint8_t *data, uint16_t dataSize);

        // DMP_CFG_1 register
        uint8_t getDMPConfig1(I2C_HandleTypeDef *i2c);
        void setDMPConfig1(I2C_HandleTypeDef *i2c, uint8_t config);

        // DMP_CFG_2 register
        uint8_t getDMPConfig2(I2C_HandleTypeDef *i2c);
        void setDMPConfig2(I2C_HandleTypeDef *i2c, uint8_t config);

		// Calibration Routines
		void CalibrateGyro(I2C_HandleTypeDef *i2c, uint8_t Loops = 15); // Fine tune after setting offsets with less Loops.
		void CalibrateAccel(I2C_HandleTypeDef *i2c, uint8_t Loops = 15);// Fine tune after setting offsets with less Loops.
		void PID(I2C_HandleTypeDef *i2c, uint8_t ReadAddress, float kP,float kI, uint8_t Loops);  // Does the math
		void PrintActiveOffsets(I2C_HandleTypeDef *i2c); // See the results of the Calibration



        // special methods for MotionApps 2.0 implementation
        #ifdef MPU6050_INCLUDE_DMP_MOTIONAPPS20

            uint8_t dmpInitialize(I2C_HandleTypeDef *i2c);
            bool dmpPacketAvailable(I2C_HandleTypeDef *i2c);

            uint8_t dmpSetFIFORate(uint8_t fifoRate);
            uint8_t dmpGetFIFORate();
            uint8_t dmpGetSampleStepSizeMS();
            uint8_t dmpGetSampleFrequency();
            int32_t dmpDecodeTemperature(int8_t tempReg);
            
            // Register callbacks after a packet of FIFO data is processed
            //uint8_t dmpRegisterFIFORateProcess(inv_obj_func func, int16_t priority);
            //uint8_t dmpUnregisterFIFORateProcess(inv_obj_func func);
            uint8_t dmpRunFIFORateProcesses();
            
            // Setup FIFO for various output
            uint8_t dmpSendQuaternion(uint_fast16_t accuracy);
            uint8_t dmpSendGyro(uint_fast16_t elements, uint_fast16_t accuracy);
            uint8_t dmpSendAccel(uint_fast16_t elements, uint_fast16_t accuracy);
            uint8_t dmpSendLinearAccel(uint_fast16_t elements, uint_fast16_t accuracy);
            uint8_t dmpSendLinearAccelInWorld(uint_fast16_t elements, uint_fast16_t accuracy);
            uint8_t dmpSendControlData(uint_fast16_t elements, uint_fast16_t accuracy);
            uint8_t dmpSendSensorData(uint_fast16_t elements, uint_fast16_t accuracy);
            uint8_t dmpSendExternalSensorData(uint_fast16_t elements, uint_fast16_t accuracy);
            uint8_t dmpSendGravity(uint_fast16_t elements, uint_fast16_t accuracy);
            uint8_t dmpSendPacketNumber(uint_fast16_t accuracy);
            uint8_t dmpSendQuantizedAccel(uint_fast16_t elements, uint_fast16_t accuracy);
            uint8_t dmpSendEIS(uint_fast16_t elements, uint_fast16_t accuracy);

            // Get Fixed Point data from FIFO
            uint8_t dmpGetAccel(int32_t *data, const uint8_t* packet=0);
            uint8_t dmpGetAccel(int16_t *data, const uint8_t* packet=0);
            uint8_t dmpGetAccel(VectorInt16 *v, const uint8_t* packet=0);
            uint8_t dmpGetQuaternion(int32_t *data, const uint8_t* packet=0);
            uint8_t dmpGetQuaternion(int16_t *data, const uint8_t* packet=0);
            uint8_t dmpGetQuaternion(Quaternion *q, const uint8_t* packet=0);
            uint8_t dmpGet6AxisQuaternion(int32_t *data, const uint8_t* packet=0);
            uint8_t dmpGet6AxisQuaternion(int16_t *data, const uint8_t* packet=0);
            uint8_t dmpGet6AxisQuaternion(Quaternion *q, const uint8_t* packet=0);
            uint8_t dmpGetRelativeQuaternion(int32_t *data, const uint8_t* packet=0);
            uint8_t dmpGetRelativeQuaternion(int16_t *data, const uint8_t* packet=0);
            uint8_t dmpGetRelativeQuaternion(Quaternion *data, const uint8_t* packet=0);
            uint8_t dmpGetGyro(int32_t *data, const uint8_t* packet=0);
            uint8_t dmpGetGyro(int16_t *data, const uint8_t* packet=0);
            uint8_t dmpGetGyro(VectorInt16 *v, const uint8_t* packet=0);
            uint8_t dmpSetLinearAccelFilterCoefficient(float coef);
            uint8_t dmpGetLinearAccel(int32_t *data, const uint8_t* packet=0);
            uint8_t dmpGetLinearAccel(int16_t *data, const uint8_t* packet=0);
            uint8_t dmpGetLinearAccel(VectorInt16 *v, const uint8_t* packet=0);
            uint8_t dmpGetLinearAccel(VectorInt16 *v, VectorInt16 *vRaw, VectorFloat *gravity);
            uint8_t dmpGetLinearAccelInWorld(int32_t *data, const uint8_t* packet=0);
            uint8_t dmpGetLinearAccelInWorld(int16_t *data, const uint8_t* packet=0);
            uint8_t dmpGetLinearAccelInWorld(VectorInt16 *v, const uint8_t* packet=0);
            uint8_t dmpGetLinearAccelInWorld(VectorInt16 *v, VectorInt16 *vReal, Quaternion *q);
            uint8_t dmpGetGyroAndAccelSensor(int32_t *data, const uint8_t* packet=0);
            uint8_t dmpGetGyroAndAccelSensor(int16_t *data, const uint8_t* packet=0);
            uint8_t dmpGetGyroAndAccelSensor(VectorInt16 *g, VectorInt16 *a, const uint8_t* packet=0);
            uint8_t dmpGetGyroSensor(int32_t *data, const uint8_t* packet=0);
            uint8_t dmpGetGyroSensor(int16_t *data, const uint8_t* packet=0);
            uint8_t dmpGetGyroSensor(VectorInt16 *v, const uint8_t* packet=0);
            uint8_t dmpGetControlData(int32_t *data, const uint8_t* packet=0);
            uint8_t dmpGetTemperature(int32_t *data, const uint8_t* packet=0);
            uint8_t dmpGetGravity(int32_t *data, const uint8_t* packet=0);
            uint8_t dmpGetGravity(int16_t *data, const uint8_t* packet=0);
            uint8_t dmpGetGravity(VectorInt16 *v, const uint8_t* packet=0);
            uint8_t dmpGetGravity(VectorFloat *v, Quaternion *q);
            uint8_t dmpGetUnquantizedAccel(int32_t *data, const uint8_t* packet=0);
            uint8_t dmpGetUnquantizedAccel(int16_t *data, const uint8_t* packet=0);
            uint8_t dmpGetUnquantizedAccel(VectorInt16 *v, const uint8_t* packet=0);
            uint8_t dmpGetQuantizedAccel(int32_t *data, const uint8_t* packet=0);
            uint8_t dmpGetQuantizedAccel(int16_t *data, const uint8_t* packet=0);
            uint8_t dmpGetQuantizedAccel(VectorInt16 *v, const uint8_t* packet=0);
            uint8_t dmpGetExternalSensorData(int32_t *data, uint16_t size, const uint8_t* packet=0);
            uint8_t dmpGetEIS(int32_t *data, const uint8_t* packet=0);
            
            uint8_t dmpGetEuler(float *data, Quaternion *q);
            uint8_t dmpGetYawPitchRoll(float *data, Quaternion *q, VectorFloat *gravity);

            // Get Floating Point data from FIFO
            uint8_t dmpGetAccelFloat(float *data, const uint8_t* packet=0);
            uint8_t dmpGetQuaternionFloat(float *data, const uint8_t* packet=0);

            uint8_t dmpProcessFIFOPacket(const unsigned char *dmpData);
            uint8_t dmpReadAndProcessFIFOPacket(I2C_HandleTypeDef *i2c, uint8_t numPackets, uint8_t *processed=NULL);

            uint8_t dmpSetFIFOProcessedCallback(void (*func) (void));

            uint8_t dmpInitFIFOParam();
            uint8_t dmpCloseFIFO();
            uint8_t dmpSetGyroDataSource(uint8_t source);
            uint8_t dmpDecodeQuantizedAccel();
            uint32_t dmpGetGyroSumOfSquare();
            uint32_t dmpGetAccelSumOfSquare();
            void dmpOverrideQuaternion(long *q);
            uint16_t dmpGetFIFOPacketSize();
            uint8_t dmpGetCurrentFIFOPacket(I2C_HandleTypeDef *i2c, uint8_t *data); // overflow proof
        #endif

        // special methods for MotionApps 4.1 implementation
        #ifdef MPU6050_INCLUDE_DMP_MOTIONAPPS41

            uint8_t dmpInitialize();
            bool dmpPacketAvailable();

            uint8_t dmpSetFIFORate(uint8_t fifoRate);
            uint8_t dmpGetFIFORate();
            uint8_t dmpGetSampleStepSizeMS();
            uint8_t dmpGetSampleFrequency();
            int32_t dmpDecodeTemperature(int8_t tempReg);
            
            // Register callbacks after a packet of FIFO data is processed
            //uint8_t dmpRegisterFIFORateProcess(inv_obj_func func, int16_t priority);
            //uint8_t dmpUnregisterFIFORateProcess(inv_obj_func func);
            uint8_t dmpRunFIFORateProcesses();
            
            // Setup FIFO for various output
            uint8_t dmpSendQuaternion(uint_fast16_t accuracy);
            uint8_t dmpSendGyro(uint_fast16_t elements, uint_fast16_t accuracy);
            uint8_t dmpSendAccel(uint_fast16_t elements, uint_fast16_t accuracy);
            uint8_t dmpSendLinearAccel(uint_fast16_t elements, uint_fast16_t accuracy);
            uint8_t dmpSendLinearAccelInWorld(uint_fast16_t elements, uint_fast16_t accuracy);
            uint8_t dmpSendControlData(uint_fast16_t elements, uint_fast16_t accuracy);
            uint8_t dmpSendSensorData(uint_fast16_t elements, uint_fast16_t accuracy);
            uint8_t dmpSendExternalSensorData(uint_fast16_t elements, uint_fast16_t accuracy);
            uint8_t dmpSendGravity(uint_fast16_t elements, uint_fast16_t accuracy);
            uint8_t dmpSendPacketNumber(uint_fast16_t accuracy);
            uint8_t dmpSendQuantizedAccel(uint_fast16_t elements, uint_fast16_t accuracy);
            uint8_t dmpSendEIS(uint_fast16_t elements, uint_fast16_t accuracy);

            // Get Fixed Point data from FIFO
            uint8_t dmpGetAccel(int32_t *data, const uint8_t* packet=0);
            uint8_t dmpGetAccel(int16_t *data, const uint8_t* packet=0);
            uint8_t dmpGetAccel(VectorInt16 *v, const uint8_t* packet=0);
            uint8_t dmpGetQuaternion(int32_t *data, const uint8_t* packet=0);
            uint8_t dmpGetQuaternion(int16_t *data, const uint8_t* packet=0);
            uint8_t dmpGetQuaternion(Quaternion *q, const uint8_t* packet=0);
            uint8_t dmpGet6AxisQuaternion(int32_t *data, const uint8_t* packet=0);
            uint8_t dmpGet6AxisQuaternion(int16_t *data, const uint8_t* packet=0);
            uint8_t dmpGet6AxisQuaternion(Quaternion *q, const uint8_t* packet=0);
            uint8_t dmpGetRelativeQuaternion(int32_t *data, const uint8_t* packet=0);
            uint8_t dmpGetRelativeQuaternion(int16_t *data, const uint8_t* packet=0);
            uint8_t dmpGetRelativeQuaternion(Quaternion *data, const uint8_t* packet=0);
            uint8_t dmpGetGyro(int32_t *data, const uint8_t* packet=0);
            uint8_t dmpGetGyro(int16_t *data, const uint8_t* packet=0);
            uint8_t dmpGetGyro(VectorInt16 *v, const uint8_t* packet=0);
            uint8_t dmpGetMag(int16_t *data, const uint8_t* packet=0);
            uint8_t dmpSetLinearAccelFilterCoefficient(float coef);
            uint8_t dmpGetLinearAccel(int32_t *data, const uint8_t* packet=0);
            uint8_t dmpGetLinearAccel(int16_t *data, const uint8_t* packet=0);
            uint8_t dmpGetLinearAccel(VectorInt16 *v, const uint8_t* packet=0);
            uint8_t dmpGetLinearAccel(VectorInt16 *v, VectorInt16 *vRaw, VectorFloat *gravity);
            uint8_t dmpGetLinearAccelInWorld(int32_t *data, const uint8_t* packet=0);
            uint8_t dmpGetLinearAccelInWorld(int16_t *data, const uint8_t* packet=0);
            uint8_t dmpGetLinearAccelInWorld(VectorInt16 *v, const uint8_t* packet=0);
            uint8_t dmpGetLinearAccelInWorld(VectorInt16 *v, VectorInt16 *vReal, Quaternion *q);
            uint8_t dmpGetGyroAndAccelSensor(int32_t *data, const uint8_t* packet=0);
            uint8_t dmpGetGyroAndAccelSensor(int16_t *data, const uint8_t* packet=0);
            uint8_t dmpGetGyroAndAccelSensor(VectorInt16 *g, VectorInt16 *a, const uint8_t* packet=0);
            uint8_t dmpGetGyroSensor(int32_t *data, const uint8_t* packet=0);
            uint8_t dmpGetGyroSensor(int16_t *data, const uint8_t* packet=0);
            uint8_t dmpGetGyroSensor(VectorInt16 *v, const uint8_t* packet=0);
            uint8_t dmpGetControlData(int32_t *data, const uint8_t* packet=0);
            uint8_t dmpGetTemperature(int32_t *data, const uint8_t* packet=0);
            uint8_t dmpGetGravity(int32_t *data, const uint8_t* packet=0);
            uint8_t dmpGetGravity(int16_t *data, const uint8_t* packet=0);
            uint8_t dmpGetGravity(VectorInt16 *v, const uint8_t* packet=0);
            uint8_t dmpGetGravity(VectorFloat *v, Quaternion *q);
            uint8_t dmpGetUnquantizedAccel(int32_t *data, const uint8_t* packet=0);
            uint8_t dmpGetUnquantizedAccel(int16_t *data, const uint8_t* packet=0);
            uint8_t dmpGetUnquantizedAccel(VectorInt16 *v, const uint8_t* packet=0);
            uint8_t dmpGetQuantizedAccel(int32_t *data, const uint8_t* packet=0);
            uint8_t dmpGetQuantizedAccel(int16_t *data, const uint8_t* packet=0);
            uint8_t dmpGetQuantizedAccel(VectorInt16 *v, const uint8_t* packet=0);
            uint8_t dmpGetExternalSensorData(int32_t *data, uint16_t size, const uint8_t* packet=0);
            uint8_t dmpGetEIS(int32_t *data, const uint8_t* packet=0);
            
            uint8_t dmpGetEuler(float *data, Quaternion *q);
            uint8_t dmpGetYawPitchRoll(float *data, Quaternion *q, VectorFloat *gravity);

            // Get Floating Point data from FIFO
            uint8_t dmpGetAccelFloat(float *data, const uint8_t* packet=0);
            uint8_t dmpGetQuaternionFloat(float *data, const uint8_t* packet=0);

            uint8_t dmpProcessFIFOPacket(const unsigned char *dmpData);
            uint8_t dmpReadAndProcessFIFOPacket(uint8_t numPackets, uint8_t *processed=NULL);

            uint8_t dmpSetFIFOProcessedCallback(void (*func) (void));

            uint8_t dmpInitFIFOParam();
            uint8_t dmpCloseFIFO();
            uint8_t dmpSetGyroDataSource(uint8_t source);
            uint8_t dmpDecodeQuantizedAccel();
            uint32_t dmpGetGyroSumOfSquare();
            uint32_t dmpGetAccelSumOfSquare();
            void dmpOverrideQuaternion(long *q);
            uint16_t dmpGetFIFOPacketSize();
        #endif

    private:
        uint8_t devAddr;
        uint8_t buffer[14];
    #if defined(MPU6050_INCLUDE_DMP_MOTIONAPPS20) or defined(MPU6050_INCLUDE_DMP_MOTIONAPPS41)
        uint8_t *dmpPacketBuffer;
        uint16_t dmpPacketSize;
    #endif
};

#endif /* _MPU6050_H_ */
