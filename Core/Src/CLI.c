#include "CLI.h"
#include "usart.h"
#include "tim.h"
#include <stdio.h>
#include <stdarg.h>
#include <string.h>
//#include "pid.h"
//#include "PID_v1.h"

uart_rx_CLI uart_hal_rx_CLI;
main_menu_t main_menu;
sub_menu_t sub_menu;
sub2_menu_t sub2_menu;

uint8_t menu_cnt = 0, submenu_cnt = 0, sub2menu_cnt = 0;


void UART4_printf(const char *fmt, ...)
{
//    char buffer[256];
//    va_list args;
//    va_start(args, fmt);
//    vsnprintf(buffer, sizeof(buffer), fmt, args);
//    va_end(args);
//    HAL_UART_Transmit(&huart4, (uint8_t*)buffer, strlen(buffer), HAL_MAX_DELAY);
}

int __io_getchar(void)
{
	unsigned char ch;
	HAL_UART_Receive(&CLI_UART, (uint8_t *)&ch, 1, HAL_MAX_DELAY);
	return ch;
}

int __io_putchar(int ch)
{
     (void) HAL_UART_Transmit(&CLI_UART, (uint8_t*) &ch, 1, HAL_MAX_DELAY);
     return ch;
}


void UART_Menu(void)
{
	printf("\r\n");
	printf("*****************************************************\n");
	printf("\r\n");
	printf("1. Control -------------------------------------> \r\n");
	printf("\r\n");
	printf("*****************************************************\n");
	printf("\r\n");
}

void UART_Menu1(void)
{
	printf("\r\n");
	printf("*****************************************************\n");
	printf("\r\n");
	printf("1. Left+Backward                               \r\n");
	printf("\r\n");
	printf("2. Backward                                    \r\n");
	printf("\r\n");
	printf("3. Right+Backward                              \r\n");
	printf("\r\n");
	printf("4. Left                                        \r\n");
	printf("\r\n");
	printf("5. Stop                                        \r\n");
	printf("\r\n");
	printf("6. Right                                       \r\n");
	printf("\r\n");
	printf("7. Left+Forward                                \r\n");
	printf("\r\n");
	printf("8. Forward                                     \r\n");
	printf("\r\n");
	printf("9. Right+Forward                               \r\n");
	printf("\r\n");
	printf("*****************************************************\n");
	printf("\r\n");
}

void Menu_Tree(uint8_t menu)
{
	switch(menu)
	{
		case Main_menu0 : UART_Menu(); 	break;
		case Main_menu1 : UART_Menu1(); break;
		default : 						break;
	}
	uint8_t CMD_str[] = "CMD>";
	HAL_UART_Transmit(&CLI_UART, (uint8_t*) CMD_str, sizeof(CMD_str), HAL_MAX_DELAY);
}

void uart_hal_rx_CLI_buffer_init(void)
{
	uart_hal_rx_CLI.cli_input_p = 0;
	uart_hal_rx_CLI.cli_output_p = 0;
	uart_hal_rx_CLI.cli_buf_p = 0;
	uart_hal_rx_CLI.cli_temp = 0;
	HAL_UART_Receive_IT(&CLI_UART, &uart_hal_rx_CLI.cli_temp, 1);

	Menu_Tree(0);
}

uint8_t uart_hal_CLI_getchar(void)
{
	uint32_t reg = READ_REG(CLI_UART.Instance->CR1);

	__HAL_UART_DISABLE_IT(&CLI_UART, UART_IT_RXNE);
	if(uart_hal_rx_CLI.cli_input_p == uart_hal_rx_CLI.cli_output_p)
	{
		WRITE_REG(CLI_UART.Instance->CR1, reg);
		return 0;
	}

	WRITE_REG(CLI_UART.Instance->CR1, reg);
	uart_hal_rx_CLI.cli_rxd = uart_hal_rx_CLI.cli_buffer[uart_hal_rx_CLI.cli_output_p++];
	if(uart_hal_rx_CLI.cli_rxd != 0x08)
	{
		uart_hal_rx_CLI.cli_buffer_temp[uart_hal_rx_CLI.cli_buf_p] = uart_hal_rx_CLI.cli_rxd;
		uart_hal_rx_CLI.cli_buf_p++;
	}
	if(uart_hal_rx_CLI.cli_output_p >= UART_RX_CLI_BUFFER_SIZE)
	{
		uart_hal_rx_CLI.cli_output_p = 0;
	}

	return 1;
}

//extern float output;
double p_gain=0.0, i_gain=0.0, d_gain=0.0;

void uart_hal_rx_CLI_monitor(void)
{
	while(uart_hal_CLI_getchar() != 0)
	{
		if(uart_hal_rx_CLI.cli_rxd == '\n')
		{
			uart_hal_rx_CLI.cli_buffer_temp[uart_hal_rx_CLI.cli_buf_p-2] = 0;	// remove \r
			uart_hal_rx_CLI.cli_buffer_temp[uart_hal_rx_CLI.cli_buf_p-1] = 0;	// remove \n
			uart_hal_rx_CLI.cli_buf_p = 0;
			if(sub2menu_cnt == Sub2_menu1)
			{

			}
			else if(sub2menu_cnt == Sub2_menu2)
			{

			}
			else
			{
				if((strcmp(uart_hal_rx_CLI.cli_buffer_temp, "q") == 0) || (strcmp(uart_hal_rx_CLI.cli_buffer_temp, "Q") == 0))
				{
					switch(menu_cnt)
					{
						case Main_menu0 : break;
						case Main_menu1 :
							menu_cnt = Main_menu0;
							submenu_cnt = Sub_menu0;
							sub2menu_cnt = Sub2_menu0;
							break;
						case Main_menu2 :
							menu_cnt = Main_menu0;
							submenu_cnt = Sub_menu0;
							sub2menu_cnt = Sub2_menu0;
							break;
						default : break;
					}
					Menu_Tree(menu_cnt);
					uart_hal_rx_CLI.cli_buf_p = 0;
				}
				else if(strcasecmp(uart_hal_rx_CLI.cli_buffer_temp, "1") == 0)
				{
					switch(menu_cnt)
					{
						case Main_menu0 :
							menu_cnt = Main_menu1;
							submenu_cnt = Sub_menu0;
							Menu_Tree(menu_cnt);
							break;
						case Main_menu1 :
//							printf("1111111\n");
//							  Modbus_ReadHoldingRegister(sID1, 0x0081, 1);
//							  Modbus_ReadHoldingRegister(sID1, 0x0089, 1);
//							Modbus_ReadHoldingRegister(sID1, 0x200E, 1);
//							__HAL_TIM_SetCompare(&htim2, TIM_CHANNEL_1, 999);
//							Kp = Kp+10;
//							p_gain = 1;
//							set_p_tune(p_gain);
							Menu_Tree(menu_cnt); break;
						default : break;
					}
				}
				else if(strcasecmp(uart_hal_rx_CLI.cli_buffer_temp, "2") == 0)
				{
					switch(menu_cnt)
					{
						case Main_menu0 : break;
						case Main_menu1 :
							printf("22222\n");
//							Modbus_WriteSingleRegister(sID1, 0x0080, 0);
//							__HAL_TIM_SetCompare(&htim2, TIM_CHANNEL_1, 1999);
//							Kp = Kp-10;
//							p_gain = -1;
//							set_p_tune(p_gain);
							Menu_Tree(menu_cnt);
							break;
						default : break;
					}
				}
				else if(strcasecmp(uart_hal_rx_CLI.cli_buffer_temp, "3") == 0)
				{
					switch(menu_cnt)
					{
						case Main_menu0 : break;
						case Main_menu1 :
//							printf("333333\n");
//							Modbus_WriteSingleRegister(sID1, 0x0040, -300);
//							HAL_TIM_Encoder_Start_IT(&htim3, TIM_CHANNEL_ALL);
//							__HAL_TIM_SetCompare(&htim2, TIM_CHANNEL_1, 3999);
//							Kd += 0.1;
//							printf("kd = %f\n", Kd);
//							i_gain = 1;
//							set_i_tune(i_gain);
							Menu_Tree(menu_cnt);
							break;
						default : break;
					}
				}
				else if(strcasecmp(uart_hal_rx_CLI.cli_buffer_temp, "4") == 0)
				{
					switch(menu_cnt)
					{
						case Main_menu0 : break;
						case Main_menu1 :
//							printf("44444\n");
//							Kd -= 0.1;
//							printf("kd = %f\n", Kd);
//							Modbus_WriteSingleRegister(sID1, 0x0040, -400);
//							HAL_TIM_Encoder_Start_IT(&htim3, TIM_CHANNEL_ALL);
//							i_gain = -1;
//							set_i_tune(i_gain);
							Menu_Tree(menu_cnt); break;
						default : break;
					}
				}
				else if(strcasecmp(uart_hal_rx_CLI.cli_buffer_temp, "5") == 0)
				{
					switch(menu_cnt)
					{
						case Main_menu1 :
//							printf("55555\n");
//							Kd += 1;
//							Modbus_WriteSingleRegister(sID1, 0x0040, 0);
//							d_gain = 1;
//							set_d_tune(d_gain);
							Menu_Tree(menu_cnt);
							break;
						default : break;
					}
				}
				else if(strcasecmp(uart_hal_rx_CLI.cli_buffer_temp, "6") == 0)
				{
					switch(menu_cnt)
					{
						case Main_menu1 :
//							printf("666666\n");
//							Kd -= 1;
//							printf("ki = %f\n", Ki);
//							Modbus_WriteSingleRegister(sID1, 0x0040, 500);
//							HAL_TIM_Encoder_Start_IT(&htim3, TIM_CHANNEL_ALL);
//							d_gain = -1;
//							set_d_tune(d_gain);
							Menu_Tree(menu_cnt);
							break;
						default : break;
					}
				}
				else if(strcasecmp(uart_hal_rx_CLI.cli_buffer_temp, "7") == 0)
				{
					switch(menu_cnt)
					{
						case Main_menu1 :
//							printf("777777\n");
//							Ki += 1;
//							Modbus_WriteSingleRegister(sID1, 0x0040, 999);
//							HAL_TIM_Encoder_Start_IT(&htim3, TIM_CHANNEL_ALL);
							Menu_Tree(menu_cnt);
							break;
						default : break;
					}
				}
				else if(strcasecmp(uart_hal_rx_CLI.cli_buffer_temp, "8") == 0)
				{
					switch(menu_cnt)
					{
						case Main_menu1 :
//							printf("88888\n");
//							Ki -= 1;
							Menu_Tree(menu_cnt);
							break;
						default : break;
					}
				}
				else if(strcasecmp(uart_hal_rx_CLI.cli_buffer_temp, "9") == 0)
				{
					switch(menu_cnt)
					{
						case Main_menu1 :
							printf("999999\n");
							Menu_Tree(menu_cnt);
							break;
						default : break;
					}
				}

//				printf("kp = %f    ki = %f   kd = %f\n", Kp, Ki, Kd);
//				output = 0;
//				PID_Init(&pid, pid.Kp, pid.Ki, pid.Kd, 0.0);

			}
		}
		else if(uart_hal_rx_CLI.cli_rxd == 0x08)
		{
			if(uart_hal_rx_CLI.cli_buf_p > 0)
			{
				uart_hal_rx_CLI.cli_buf_p--;
				uart_hal_rx_CLI.cli_buffer_temp[uart_hal_rx_CLI.cli_buf_p] = 0;
			}
		}
	}
}










