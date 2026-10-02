#ifndef INC_CLI_H_
#define INC_CLI_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "main.h"


#define CLI_UART						huart1
#define UART_RX_CLI_BUFFER_SIZE			50

typedef struct
{
	char cli_buffer[UART_RX_CLI_BUFFER_SIZE];
	char cli_buffer_temp[UART_RX_CLI_BUFFER_SIZE];
	uint8_t cli_rxd;
	volatile uint16_t cli_input_p;
	volatile uint16_t cli_output_p;
	volatile uint16_t cli_buf_p;
	uint8_t cli_temp;
} uart_rx_CLI;
extern uart_rx_CLI uart_hal_rx_CLI;

typedef enum main_menu_e
{
    Main_menu0 = 0,
	Main_menu1,
	Main_menu2,
	Main_menu3,
} main_menu_t;
extern main_menu_t main_menu;

typedef enum sub_menu_e
{
	Sub_menu0 = 0,
	Sub_menu1,
	Sub_menu2,
	Sub_menu3,
	Sub_menu4,
	Sub_menu5,
	Sub_menu6,
	Sub_menu7,
	Sub_menu8,
	Sub_menu9,
	Sub_menu10,
} sub_menu_t;
extern sub_menu_t sub_menu;

typedef enum sub2_menu_e
{
	Sub2_menu0 = 0,
	Sub2_menu1,
	Sub2_menu2,
	Sub2_menu3,
	Sub2_menu4,
	Sub2_menu5,
	Sub2_menu6,
	Sub2_menu7,
	Sub2_menu8,
	Sub2_menu9,
	Sub2_menu10,
} sub2_menu_t;
extern sub2_menu_t sub2_menu;

extern uint8_t menu_cnt, submenu_cnt, sub2menu_cnt;

void UART4_printf(const char *fmt, ...);
int __io_getchar(void);
int __io_putchar(int ch);
void UART_Menu(void);
void UART_Menu1(void);
void Menu_Tree(uint8_t menu);
void uart_hal_rx_CLI_buffer_init(void);
uint8_t uart_hal_CLI_getchar(void);
void uart_hal_rx_CLI_monitor(void);

#ifdef __cplusplus
}
#endif

#endif /* INC_CLI_H_ */
