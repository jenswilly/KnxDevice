/*
 * TimeUtils.h
 *
 *  Created on: 24 Oct 2019
 *      Author: jenswilly
 */

#ifndef KNXDEVICE_TIMEUTILS_H_
#define KNXDEVICE_TIMEUTILS_H_

#include "lpc_types.h"

class TimeUtils
{
public:
	inline static uint16_t millis() { return 0; }
	inline static uint16_t micros() { return 0; }

private:
	// Static-only class
	TimeUtils() {}
};

#endif /* KNXDEVICE_TIMEUTILS_H_ */
