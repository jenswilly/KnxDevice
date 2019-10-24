/*
 * HardwareSerial.h
 *
 *  Created on: 24 Oct 2019
 *      Author: jenswilly
 */

#ifndef KNXDEVICE_HARDWARESERIAL_H_
#define KNXDEVICE_HARDWARESERIAL_H_

#include "lpc_types.h"

enum e_SerialMode
{
	SERIAL_8E1
};

class HardwareSerial
{
public:
	HardwareSerial();
	~HardwareSerial();

	virtual int available() { return 0; }
    virtual int peek() { return 0; }
	virtual uint8_t read() { return 0; }

    virtual void begin( unsigned long baud, e_SerialMode mode ) {}
	virtual void end() {}

    virtual int write( uint8_t data ) { return 1; };
    virtual void flush() {}

    int write( uint8_t *buffer, int size );
};

#endif /* KNXDEVICE_HARDWARESERIAL_H_ */
