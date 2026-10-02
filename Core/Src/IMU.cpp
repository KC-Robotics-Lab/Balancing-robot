#include "MPU6050_6Axis_MotionApps20.h"
#include "main.h"
#include "i2c.h"
#include "tim.h"
#include "PID_v1.h"
#include "cmsis_os.h"

#include "ModbusRTU_Master.h"
#include "ModbusRTU_app.h"

extern osMessageQueueId_t IMUAngleHandle;

MPU6050 mpu;

double input, output, setpoint = 0.0;
//double Kp = 40.0;
//double Kd = 1.4;
//double Ki = 60.0;
#if 0
double Kp = 60.0;
double Kd = 1.4;
double Ki = 150.0;
#else
//double Kp = 45.0;
//double Kd = 2.5;
//double Ki = 60.0;
//double Kp = 40.0;
//double Kd = 1.4;
//double Ki = 200.0;
double Kp = 40.0;
double Kd = 1.4;
double Ki = 60.0;
#endif
double out_tmp;

PID pid(&input, &output, &setpoint, Kp, Ki, Kd, DIRECT);

bool dmpReady0 = false;
uint8_t mpuIntStatus0;   	// holds actual interrupt status byte from MPU
uint8_t devStatus0;      	// return status after each device operation (0 = success, !0 = error)
uint16_t packetSize0;    	// expected DMP packet size (default is 42 bytes)
uint16_t fifoCount0;     	// count of all bytes currently in FIFO
uint8_t fifoBuffer0[1024]; 	// FIFO storage buffer

Quaternion q0;           	// [w, x, y, z]         quaternion container
VectorFloat gravity0;    	// [x, y, z]            gravity vector
float ypr0[3];           	// [yaw, pitch, roll]   yaw/pitch/roll container and gravity vector


void IMU0_Setup()
{
	printf(F("Initializing I2C devices..."));
    mpu.initialize(&hi2c2);

    printf(F("Testing device connections...\n"));
    printf(mpu.testConnection(&hi2c2) ? F("MPU6050 connection successful\n") : F("MPU6050 connection failed\n"));
//    mpu.setXGyroOffset(&hi2c2, 220);
//    mpu.setYGyroOffset(&hi2c2, 76);
//    mpu.setZGyroOffset(&hi2c2, -85);
//    mpu.setZAccelOffset(&hi2c2, 1788);

    mpu.setXAccelOffset(&hi2c2, 392);
    mpu.setYAccelOffset(&hi2c2, 2277);
    mpu.setZAccelOffset(&hi2c2, 1486);
    mpu.setXGyroOffset(&hi2c2, 51);
    mpu.setYGyroOffset(&hi2c2, -48);
    mpu.setZGyroOffset(&hi2c2, 4);

    printf(F("Initializing DMP...\n"));
    devStatus0 = mpu.dmpInitialize(&hi2c2);
    if (devStatus0 == 0)
    {
    	printf(F("Enabling DMP...\n"));
        mpu.setDMPEnabled(&hi2c2, true);
        devStatus0 = mpu.getIntStatus(&hi2c2);
        printf(F("DMP ready! Waiting for first interrupt...\n"));
        dmpReady0 = true;
        packetSize0 = mpu.dmpGetFIFOPacketSize();

        pid.SetMode(AUTOMATIC);
        pid.SetSampleTime(10);
        pid.SetOutputLimits(-255, 255);
    }
    else
    {
        // ERROR!
        // 1 = initial memory load failed
        // 2 = DMP configuration updates failed
        // (if it's going to break, usually the code will be 1)
        printf(F("DMP Initialization failed (code "));
        printf("%d\n",devStatus0);
    }
}

//int32_t aaaa[2] = {0,};

void IMU0_Proc(float *deg)
{
	if (!dmpReady0) return;

	while (fifoCount0 < packetSize0)
	{
		osMessageQueuePut(IMUAngleHandle, &input, 0, 0);
    	pid.Compute();
//    	printf(" %f    %f    %d\n", input, output, fifoCount0);
    	out_tmp = mapArduino_double(output, -255, 255, -999, 999);
//    	out_tmp = mapArduino_double(output, -255, 255, -1999, 1999);

//		if(out_tmp < 0)
//		{
//			HAL_GPIO_WritePin(DIR1_GPIO_Port, DIR1_Pin, GPIO_PIN_SET);
//			__HAL_TIM_SetCompare(&htim2, TIM_CHANNEL_1, out_tmp*-1);
//			HAL_GPIO_WritePin(DIR2_GPIO_Port, DIR2_Pin, GPIO_PIN_SET);
//			__HAL_TIM_SetCompare(&htim2, TIM_CHANNEL_4, out_tmp*-1);
//		}
//		else if(out_tmp > 0)
//		{
//			HAL_GPIO_WritePin(DIR1_GPIO_Port, DIR1_Pin, GPIO_PIN_RESET);
//			__HAL_TIM_SetCompare(&htim2, TIM_CHANNEL_1, out_tmp*1);
//			HAL_GPIO_WritePin(DIR2_GPIO_Port, DIR2_Pin, GPIO_PIN_RESET);
//			__HAL_TIM_SetCompare(&htim2, TIM_CHANNEL_4, out_tmp*1);
//    	}
//		else
//		{
////			__HAL_TIM_SetCompare(&htim2, TIM_CHANNEL_1, 0);
////			__HAL_TIM_SetCompare(&htim2, TIM_CHANNEL_4, 0);
//		}

		fifoCount0 = mpu.getFIFOCount(&hi2c2);

//		Modbus_ReadHoldingRegister(0x01, 0x20A7, 4);
//		aaaa[0] = ((int32_t)(ModbusRegister[0] << 16) | ModbusRegister[1]);
//		aaaa[1] = ((int32_t)(ModbusRegister[2] << 16) | ModbusRegister[3]);

		osDelay(1);
	}

	if (fifoCount0 == 1024)
	{
		mpu.resetFIFO(&hi2c2);
		printf("FIFO overflow!");
	}
	else
	{
		if (fifoCount0 % packetSize0 != 0)
		{
			mpu.resetFIFO(&hi2c2);
		}
		else
		{
			while (fifoCount0 >= packetSize0)
			{
				mpu.getFIFOBytes(&hi2c2, fifoBuffer0, packetSize0);
				fifoCount0 -= packetSize0;
			}

			mpu.dmpGetQuaternion(&q0,fifoBuffer0);
			mpu.dmpGetGravity(&gravity0,&q0);
			mpu.dmpGetYawPitchRoll(ypr0, &q0, &gravity0);

			#if LOG_INPUT
				Serial.print("ypr\t");
				Serial.print(ypr[0] * 180/M_PI);
				Serial.print("\t");
				Serial.print(ypr[1] * 180/M_PI);
				Serial.print("\t");
				Serial.println(ypr[2] * 180/M_PI);
			#endif
			input = (ypr0[2] * 180 / M_PI);
		}
	}
}

