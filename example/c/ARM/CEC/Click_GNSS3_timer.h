/*

Use mikroE Timer Calculator to generate proper timer configuration
and timer ISR.

https://www.mikroe.com/timer-calculator

*/
#include "Click_GNSS3_types.h"

uint32_t timerCounter = 0;

static void gnss3_configTimer()
{


    // Configure Timer
}

void Timer_interrupt()
{
    gnss3_tick();
	timerCounter++;
    // Reset Flag
}