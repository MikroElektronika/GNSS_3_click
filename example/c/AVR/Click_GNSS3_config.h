#include "Click_GNSS3_types.h"

const uint32_t _GNSS3_TIMER_LIMIT      = 5;	    // 5 ticks
const uint16_t _GNSS3_BUF_WARNING      = 192;	// 192 bytes activate warning
const uint8_t  _GNSS3_POLL_ENABLE      = 1; 	// poll enabled
const uint8_t  _GNSS3_CALLBACK_ENABLE  = 0;	    // calback disabled

const uint32_t _GNSS3_UART_CFG[ 3 ] = 
{
	115200, 
	_UART_NOPARITY, 
	_UART_ONE_STOPBIT
};