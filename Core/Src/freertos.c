/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : freertos.c
  * Description        : Code for freertos applications
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "tim.h"
#include "MicroRos.h"
#include "ModbusRTU_Master.h"
#include "ModbusRTU_app.h"
#include "CLI.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */
osThreadId defaultTaskHandle;
/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN Variables */
int16_t FB_ros_data, LR_ros_data;
int16_t LR_Last_data, FB_Last_data;
extern uint8_t mode;
extern bool motor_disable;
/* USER CODE END Variables */
/* Definitions for MicroRos */
osThreadId_t MicroRosHandle;
const osThreadAttr_t MicroRos_attributes = {
  .name = "MicroRos",
  .stack_size = 3000 * 4,
  .priority = (osPriority_t) osPriorityAboveNormal,
};
/* Definitions for MotorTask */
osThreadId_t MotorTaskHandle;
const osThreadAttr_t MotorTask_attributes = {
  .name = "MotorTask",
  .stack_size = 512 * 4,
  .priority = (osPriority_t) osPriorityBelowNormal,
};
/* Definitions for BalancingTask */
osThreadId_t BalancingTaskHandle;
const osThreadAttr_t BalancingTask_attributes = {
  .name = "BalancingTask",
  .stack_size = 512 * 4,
  .priority = (osPriority_t) osPriorityAboveNormal1,
};
/* Definitions for CLITask */
osThreadId_t CLITaskHandle;
const osThreadAttr_t CLITask_attributes = {
  .name = "CLITask",
  .stack_size = 512 * 4,
  .priority = (osPriority_t) osPriorityBelowNormal,
};
/* Definitions for IMUAngle */
osMessageQueueId_t IMUAngleHandle;
const osMessageQueueAttr_t IMUAngle_attributes = {
  .name = "IMUAngle"
};
/* Definitions for CLI */
osSemaphoreId_t CLIHandle;
const osSemaphoreAttr_t CLI_attributes = {
  .name = "CLI"
};

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */
extern void ModbusRTUTask(void const * argument);
/* USER CODE END FunctionPrototypes */

void StartMicroRos(void *argument);
void StartMotorTask(void *argument);
void StartBalancingTask(void *argument);
void StartCLITask(void *argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/**
  * @brief  FreeRTOS initialization
  * @param  None
  * @retval None
  */
void MX_FREERTOS_Init(void) {
  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* Create the semaphores(s) */
  /* creation of CLI */
  CLIHandle = osSemaphoreNew(1, 0, &CLI_attributes);

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* Create the queue(s) */
  /* creation of IMUAngle */
  IMUAngleHandle = osMessageQueueNew (1, sizeof(double), &IMUAngle_attributes);

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* creation of MicroRos */
  MicroRosHandle = osThreadNew(StartMicroRos, NULL, &MicroRos_attributes);

  /* creation of MotorTask */
  MotorTaskHandle = osThreadNew(StartMotorTask, NULL, &MotorTask_attributes);

  /* creation of BalancingTask */
  BalancingTaskHandle = osThreadNew(StartBalancingTask, NULL, &BalancingTask_attributes);

  /* creation of CLITask */
  CLITaskHandle = osThreadNew(StartCLITask, NULL, &CLITask_attributes);

  /* USER CODE BEGIN RTOS_THREADS */

  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

  /* USER CODE BEGIN RTOS_EVENTS */
  /* add events, ... */

  /* USER CODE END RTOS_EVENTS */

}

/* USER CODE BEGIN Header_StartMicroRos */
/**
  * @brief  Function implementing the MicroRos thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_StartMicroRos */
void StartMicroRos(void *argument)
{
  /* USER CODE BEGIN StartMicroRos */
	Micro_ros_Init();
	Micro_ros_PUB_Init();
	Micro_ros_SUB_Init();
	Micro_ros_service_Init();

  /* Infinite loop */
  for(;;)
  {
		Micro_ros_Proc();
		Micro_ros_spin(1);

    osDelay(1);
  }
  /* USER CODE END StartMicroRos */
}

/* USER CODE BEGIN Header_StartMotorTask */
/**
* @brief Function implementing the MotorTask thread.
* @param argument: Not used
* @retval None
*/
int32_t aaaa[2] = {0,};
/* USER CODE END Header_StartMotorTask */
void StartMotorTask(void *argument)
{
  /* USER CODE BEGIN StartMotorTask */

	Motor_driver_Init();
  /* Infinite loop */
  for(;;)
  {
		Modbus_ReadHoldingRegister(0x01, 0x20A7, 4);
		aaaa[0] = ((int32_t)(ModbusRegister[0] << 16) | ModbusRegister[1]);
		aaaa[1] = ((int32_t)(ModbusRegister[2] << 16) | ModbusRegister[3]);
	  if(mode == 2)
	  {
		  Set_cmd_vel_rpm();
		  mode = 0;
	  }
	  else if(mode == 1)
	  {
		  Run_joycon();
		  mode = 0;
	  }

	  Disable_control(motor_disable);

    osDelay(1);
  }
  /* USER CODE END StartMotorTask */
}

/* USER CODE BEGIN Header_StartBalancingTask */
/**
* @brief Function implementing the BalancingTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartBalancingTask */
void StartBalancingTask(void *argument)
{
  /* USER CODE BEGIN StartBalancingTask */
	float degree[3];
	HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_1);
	HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_4);

	IMU0_Setup();
  /* Infinite loop */
  for(;;)
  {
//	  vTaskDelete(NULL);
//	  IMU0_Proc(degree);
    osDelay(1);
  }
  /* USER CODE END StartBalancingTask */
}

/* USER CODE BEGIN Header_StartCLITask */
/**
* @brief Function implementing the CLITask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartCLITask */
void StartCLITask(void *argument)
{
  /* USER CODE BEGIN StartCLITask */
//	uart_hal_rx_CLI_buffer_init();
  /* Infinite loop */
  for(;;)
  {
//	  if(osSemaphoreAcquire(CLIHandle, 0) == osOK)
//	  {
//		  uart_hal_rx_CLI_monitor();
//	  }
    osDelay(1);
  }
  /* USER CODE END StartCLITask */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* USER CODE END Application */

