#include <hal/include/libopencm3/stm32/rcc.h>
#include <hal/include/libopencm3/stm32/gpio.h>
#include <hal/include/libopencm3/stm32/usart.h>

#include "uart.h"

int uart_init(uart_config_t config)
{
	uint32_t usart;
	uint32_t baud;
	uint32_t stopbits;
	uint32_t parity;
	uint32_t mode;
	uint32_t bits;
	uint32_t flowcontrol;

	switch(config.port)
	{
	case UART_PORT_1:
	  usart = USART1_BASE;
	  rcc_periph_clock_enable(RCC_GPIOA);
	  rcc_periph_clock_enable(RCC_USART1);
	  gpio_set_mode(
			GPIOA,
			GPIO_MODE_OUTPUT_50MHZ,
			GPIO_CNF_OUTPUT_ALTFN_PUSHPULL,
			GPIO_USART1_TX);
	  break;
	case UART_PORT_2:
		  usart = USART2_BASE;
		  //TODO 
		  break;
	case UART_PORT_3:
	  usart = USART3_BASE;
	  //TODO 
	  break;
	case UART_PORT_4:
	  usart = UART4_BASE;
	  //TODO 
	  break;
	case UART_PORT_5:
	  usart = UART5_BASE;
	  //TODO 
	  break;
	default:
	  //TODO Add error log system.
	  break;

	}

	
	baud = (uint32_t) config.baud_rate;

	switch(config.stop_bits)
	{
	case UART_SB_0_5:        stopbits = USART_STOPBITS_0_5;   break;
	case UART_SB_1:          stopbits = USART_STOPBITS_1;     break;
	case UART_SB_1_5:        stopbits = USART_STOPBITS_1_5;   break;
	case USART_STOPBITS_2:   stopbits = USART_STOPBITS_2;     break;
	default:
	  //TODO Add error log system.
	  break;

	}

	switch(config.parity)
	{
	case UART_NO_PARITY:     parity = USART_PARITY_NONE;   break;
	case UART_ODD_PARITY:    parity = USART_PARITY_ODD;    break;
	case UART_EVEN_PARITY:   parity = USART_PARITY_EVEN;   break;
	default:
	  //TODO Add error log system.
	  break;


	switch(config.mode)
	{
	case UART_MODE_TX:       mode = USART_MODE_TX;      break;
	case UART_MODE_RX:       mode = USART_MODE_RX;      break;
	case UART_MODE_TX_RX:    mode = USART_MODE_TX_RX;   break;
	default:
	  //TODO Add error log system.
	  break;

	}

	switch(config.word_length)
	{
	case UART_WL_8:   bits = 8;   break;
	case UART_WL_9:   bits = 9;   break;
	default:
	  //TODO Add error log system.
	  break;

	}

	switch(config.flow_control)
	{
	case UART_FC_NONE:      flowcontrol = USART_FLOWCONTROL_NONE;      break; 
	case UART_FC_RTS:       flowcontrol = USART_FLOWCONTROL_RTS;       break;
	case UART_FC_CTS:       flowcontrol = USART_FLOWCONTROL_CTS;       break;
	case UART_FC_RTS_CTS:   flowcontrol = USART_FLOWCONTROL_RTS_CTS;   break;
	default:
	  //TODO Add error log system.
	  break;

	}

	/* Setup UART parameters. */
	usart_set_baudrate(usart, baud);
	usart_set_databits(usart, bits);
	usart_set_stopbits(usart, stopbits);
	usart_set_mode(usart, mode);
	usart_set_parity(usart, parity);
	usart_set_flow_control(usart, flowcontrol);

	/* Enable UART. */
	usart_enable(usart);
	
}
