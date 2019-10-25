/*
 * TimeUtils.h
 *
 *  Created on: 24 Oct 2019
 *      Author: jenswilly
 */

#ifndef KNXDEVICE_TIMEUTILS_H_
#define KNXDEVICE_TIMEUTILS_H_

#include "board.h"

class TimeUtils
{
public:
	static void init();
	inline static uint32_t millis() { return TimeUtils::micros() / 1000; }
	inline static uint32_t micros() { return Chip_TIMER_ReadCount( LPC_TIMER32_0 ); }	// The timer is counting at 1 MHz so just return current value.
	inline static uint32_t TimeDelta( uint32_t now, uint32_t before ) { return now - before; }	// This will work even with rollover

private:
	// Static-only class
	TimeUtils() {}
};

#endif /* KNXDEVICE_TIMEUTILS_H_ */
