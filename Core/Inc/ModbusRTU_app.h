#ifndef INC_MODBUSRTU_APP_H_
#define INC_MODBUSRTU_APP_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "main.h"

void Motor_driver_Init(void);
void Motor_Torque_Disable(void);
void Alram_Clear(void);
void Velocity_Mode_Init(void);
void Encoder_RST(uint8_t wheel);
void Disable_control(bool en);
void Cmd_Wheel_Velocity_Control(int16_t *velocity);
void Left_Motor_Velocity_Control(int16_t velocity);
void Right_Motor_Velocity_Control(int16_t velocity);
void Synchronous_Velocity_Control(int16_t velocity);
void Asynchronous_Velocity_Control(int16_t velocity);
void Left_Asynchronous_Velocity_Control(int16_t velocity);
void Right_Asynchronous_Velocity_Control(int16_t velocity);
void LR_Synchronous_Velocity_Control(int16_t velocity);
void Left_Synchronous_Velocity_Control(int16_t velocity);
void Right_Synchronous_Velocity_Control(int16_t velocity);
void Stop_Velocity_Control(void);
void Quick_Stop_Velocity_Control(void);

void Set_cmd_vel_rpm(void);
void Run_joycon(void);

#ifdef __cplusplus
}
#endif

#endif /* INC_MODBUSRTU_APP_H_ */
