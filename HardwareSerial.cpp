/*
 * HardwareSerial.cpp
 *
 *  Created on: 24 Oct 2019
 *      Author: jenswilly
 */

#include "HardwareSerial.h"

HardwareSerial::HardwareSerial()
{
	// TODO Auto-generated constructor stub
}

HardwareSerial::~HardwareSerial()
{
}

int HardwareSerial::write( uint8_t *buffer, int size )
{
	while( size )
		write( buffer[size--] );

	return size;
}

