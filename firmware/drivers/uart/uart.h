#ifndef UART_H
#define UART_H

/**
 * \brief UART ports.
 */
typedef enum
{
	UART_PORT_1=0,          /**< UART port 1. */
	UART_PORT_2,            /**< UART port 2. */
	UART_PORT_3,            /**< UART port 3. */
	UART_PORT_4,            /**< UART port 4. */
	UART_PORT_5             /**< UART port 5. */
} uart_ports_e;

/**
 * \brief UART modes.
 */
typedef enum 
{
	UART_MODE_TX=0,
	UART_MODE_RX,
	UART_MODE_TX_RX	
} uart_modes_e;

/**
 * \brief Parity configuration.
 */
typedef enum
{
    UART_NO_PARITY=0,       /**< No parity. */
    UART_ODD_PARITY,        /**< Odd parity. */
    UART_EVEN_PARITY        /**< Even parity. */
} uart_parity_e;	

/**
 * \brief Stop bits configuration.
 */
typedef enum 
{
	UART_SB_0_5=0,
	UART_SB_1,
	UART_SB_1_5,
	UART_SB_2	
} uart_sb_e;

/**
 *  \brief Word length configuration.
 */
typedef enum
{
	UART_WL_8=0,
	UART_WL_9
} uart_wl_e;


/**
 *  \brief Hardware Flow Control.
 */
typedef enum 
{
	UART_FC_NONE=0,
	UART_FC_RTS,
	UART_FC_CTS,
	UART_FC_RTS_CTS
} uart_fc_e;

/**
 *  \brief Baud rate configuration.
 */
typedef uint32_t uart_br_t;

typedef struct
{
	uart_ports_e port;
	uart_modes_e mode;
	uart_parity_e parity;
	uart_sb_e stop_bits;
	uart_wl_e word_length;
	uart_fc_e flow_control;
	uart_br_t baud_rate;
} uart_config_t;

int uart_init(uart_config_t config);


#endif /* UART_H */
