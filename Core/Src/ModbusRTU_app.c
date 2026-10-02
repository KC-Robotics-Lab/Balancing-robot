#include "ModbusRTU_app.h"
#include "ModbusRTU_Master.h"
#include "cmsis_os.h"

extern osMessageQueueId_t Wheel0Handle;
extern osMessageQueueId_t Wheel1Handle;
extern osMessageQueueId_t FB_dataHandle;
extern osMessageQueueId_t LR_dataHandle;

extern int16_t FB_ros_data, LR_ros_data;
extern int16_t LR_Last_data, FB_Last_data;
int16_t Wheel_data[2], Wheel_Last_data[2];

bool motor_disable = false;

void Motor_driver_Init(void)
{
	Alram_Clear();
	Stop_Velocity_Control();
	Velocity_Mode_Init();
	Encoder_RST(3);
}

void Motor_Torque_Disable(void)
{
	Modbus_WriteSingleRegister(0x01, 0x200E, 0x0007);
}

void Alram_Clear(void)
{
	Modbus_WriteSingleRegister(0x01, 0x200E, 0x0006);
//	Modbus_WriteSingleRegister(0x01, 0x200E, 0x0006);
}

void Velocity_Mode_Init(void)
{
	uint16_t data1[4] = {500, 500, 500, 500};
	Modbus_WriteMultipleRegisters(0x01, 0x2080, 4, data1);
	uint16_t data2[2] = {0x0003, 0x0008};
	Modbus_WriteMultipleRegisters(0x01, 0x200D, 2, data2);
//	Modbus_WriteMultipleRegisters(0x01, 0x200D, 2, data2);
}

void Encoder_RST(uint8_t wheel)
{
	Modbus_WriteSingleRegister(0x01, 0x2005, wheel);
//	Modbus_WriteSingleRegister(0x01, 0x2005, wheel);
}

void Disable_control(bool en)
{
	if(en == true)
	{
		Modbus_WriteSingleRegister(0x01, 0x200E, 0x0007);
//		Modbus_WriteSingleRegister(0x01, 0x200E, 0x0007);
		motor_disable = false;
	}
}
#ifdef AMR_Cmd_Vel
void Cmd_Wheel_Velocity_Control(int16_t *velocity)
{
	uint16_t data1[2] = {velocity[0], velocity[1]};
	Modbus_WriteMultipleRegisters(0x01, 0x2088, 2, data1);
}
#endif
void Left_Motor_Velocity_Control(int16_t velocity)
{
	Modbus_WriteSingleRegister(0x01, 0x2088, velocity);
}

void Right_Motor_Velocity_Control(int16_t velocity)
{
	Modbus_WriteSingleRegister(0x01, 0x2089, velocity);
}

void Synchronous_Velocity_Control(int16_t velocity)
{
	uint16_t data1[2] = {velocity, (~velocity) + 1};
	Modbus_WriteMultipleRegisters(0x01, 0x2088, 2, data1);
}

void Asynchronous_Velocity_Control(int16_t velocity)
{
	int16_t data;
	if(FB_ros_data > 0)
	{
		if(LR_ros_data > 0) data = velocity;
		else if(LR_ros_data < 0) data = velocity*-1;
	}
	else if(FB_ros_data < 0)
	{
		if(LR_ros_data > 0) data = velocity*-1;
		else if(LR_ros_data < 0) data = velocity;
	}

	if(velocity > 0)
	{
		Left_Asynchronous_Velocity_Control(data);
	}
	else if(velocity < 0)
	{
		Right_Asynchronous_Velocity_Control(data);
	}
	else
		Stop_Velocity_Control();
}

void Left_Asynchronous_Velocity_Control(int16_t velocity)
{
	uint16_t data1[2] = {velocity/2, (~velocity) + 1};
	//uint16_t data1[2] = {velocity/3, (~velocity) + 1};
	Modbus_WriteMultipleRegisters(0x01, 0x2088, 2, data1);
}

void Right_Asynchronous_Velocity_Control(int16_t velocity)
{
	uint16_t data1[2] = {velocity, ((~velocity) + 1)/2};
	//uint16_t data1[2] = {((~velocity) + 1), velocity/3};
	Modbus_WriteMultipleRegisters(0x01, 0x2088, 2, data1);
}

void LR_Synchronous_Velocity_Control(int16_t velocity)
{
	if(velocity > 0)
	{
		Left_Synchronous_Velocity_Control(velocity);
	}
	else if(velocity < 0)
	{
		Right_Synchronous_Velocity_Control(velocity);
	}
	else
		Stop_Velocity_Control();
}

void Left_Synchronous_Velocity_Control(int16_t velocity)
{
	//int16_t data1[2] = {(~velocity) + 1, (~velocity) + 1};
	uint16_t data1[2] = {velocity*-1, velocity*-1};
	Modbus_WriteMultipleRegisters(0x01, 0x2088, 2, data1);
}

void Right_Synchronous_Velocity_Control(int16_t velocity)
{
	uint16_t data1[2] = {velocity*-1, velocity*-1};
	Modbus_WriteMultipleRegisters(0x01, 0x2088, 2, data1);
}

void Stop_Velocity_Control(void)
{
	uint16_t data1[2] = {0, 0};
	Modbus_WriteMultipleRegisters(0x01, 0x2088, 2, data1);
}

void Quick_Stop_Velocity_Control(void)
{
	Modbus_WriteSingleRegister(0x01, 0x200E, 0x0005);
}

void Cmd_Wheel_Velocity_Control(int16_t *velocity)
{
	uint16_t data1[2] = {velocity[0], velocity[1]};
	Modbus_WriteMultipleRegisters(0x01, 0x2088, 2, data1);
}

void Set_cmd_vel_rpm(void)
{
//	int16_t Wheel_data[2];
//	osMessageQueueGet(Wheel0Handle, &Wheel_data[0], NULL, 0);
//	osMessageQueueGet(Wheel1Handle, &Wheel_data[1], NULL, 0);
	if((Wheel_data[0] != Wheel_Last_data[0]) || (Wheel_data[1] != Wheel_Last_data[1]))
	{
		Cmd_Wheel_Velocity_Control(Wheel_data);
		Wheel_Last_data[0] = Wheel_data[0];
		Wheel_Last_data[1] = Wheel_data[1];
	}
}

void Run_joycon(void)
{
//	osMessageQueueGet(FB_dataHandle, &data[0], NULL, 0);
//	osMessageQueueGet(LR_dataHandle, &data[1], NULL, 0);
	if(FB_ros_data != FB_Last_data)
	{
		if(FB_ros_data == 0)
		{
			LR_Synchronous_Velocity_Control(LR_ros_data);
		}
		else
		{
			if(LR_ros_data == 0)
			{
				Synchronous_Velocity_Control(FB_ros_data);
			}
			else
			{
				Asynchronous_Velocity_Control(LR_ros_data);
			}
		}
		FB_Last_data = FB_ros_data;
	}
	else if(LR_ros_data != LR_Last_data)
	{
		if(LR_ros_data == 0)
		{
			Synchronous_Velocity_Control(FB_ros_data);
		}
		else
		{
			if(FB_ros_data == 0)
			{
				LR_Synchronous_Velocity_Control(LR_ros_data);			//Only left or right
			}
			else
			{
				Asynchronous_Velocity_Control(LR_ros_data);				//F/B + L/R
			}
		}
		LR_Last_data = LR_ros_data;
	}
}









