/*
 * TimeUtils.cpp
 *
 *  Created on: 24 Oct 2019
 *      Author: jenswilly
 */

#include "TimeUtils.h"

/**
 * Initializes the timer LPC_TIMER32_0 for microsecond counting.
 */
void TimeUtils::init()
{
	Chip_TIMER_Init( LPC_TIMER32_0 );
	Chip_TIMER_Reset( LPC_TIMER32_0 );

	uint32_t prescaler = SystemCoreClock / 1000000 - 1;	// Set prescaler for 1 MHz frequency
	Chip_TIMER_PrescaleSet( LPC_TIMER32_0, prescaler );

	// Start the timers
	Chip_TIMER_Enable( LPC_TIMER32_0 );
}
