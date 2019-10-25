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

/**
 * Abstract (mostly, anyway) serial interface class.
 *
 * Implement all the virtual functions in
 */
class HardwareSerial
{
public:
	HardwareSerial();
	~HardwareSerial();

	virtual int available() = 0;
	virtual uint8_t read() = 0;

    virtual void begin( unsigned long baud, e_SerialMode mode ) = 0;
	virtual void end() = 0;

    virtual int write( uint8_t data ) = 0;
    virtual int write( uint8_t *buffer, int size );	// This method has a default implementation calling write(uint8_t)
};

#endif /* KNXDEVICE_HARDWARESERIAL_H_ */
