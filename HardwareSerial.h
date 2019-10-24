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

	inline int available()
	{
		return 0;
	}

	inline uint8_t read()
	{
		return 0;
	}

	inline void end() {}

    inline int peek()
    {
    	return 0;
    }

    inline void flush() {}

    inline int write( uint8_t data )
    {
    	return 1;
    }
    inline int write( unsigned long n ) { return write( (uint8_t)n ); }
    inline int write( long n ) { return write( (uint8_t)n ); }
    inline int write( unsigned int n ) { return write( (uint8_t)n ); }
    inline int write( int n ) { return write( (uint8_t)n ); }

    inline int write( uint8_t *buffer, int size )
    {
    	while( size )
    		write( buffer[size--] );

    	return size;
    }

    inline void begin( unsigned long baud, e_SerialMode mode ) {}
};

#endif /* KNXDEVICE_HARDWARESERIAL_H_ */
