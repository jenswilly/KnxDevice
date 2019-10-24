//    This file is part of Arduino Knx Bus Device library.

//    The Arduino Knx Bus Device library allows to turn Arduino into "self-made" KNX bus device.
//    Copyright (C) 2014 2015 2016 Franck MARINI (fm@liwan.fr)

//    The Arduino Knx Bus Device library is free software: you can redistribute it and/or modify
//    it under the terms of the GNU General Public License as published by
//    the Free Software Foundation, either version 3 of the License, or
//    (at your option) any later version.

//    This program is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//    GNU General Public License for more details.

//    You should have received a copy of the GNU General Public License
//    along with this program.  If not, see <http://www.gnu.org/licenses/>.


// File : KnxTelegram.h
// Author : Franck Marini
// Description : Handling of the KNX Telegrams
// Module dependencies : none

// Modified:
// 2019-10, JWJ

#include "KnxTelegram.h"
#include <iomanip>

KnxTelegram::KnxTelegram() { ClearTelegram(); }; // Clear telegram with default values

void KnxTelegram::ClearTelegram(void)
{
// clear telegram with default values :
// std FF, no repeat, normal prio, empty payload
// multicast, routing counter = 6, payload length = 1
  for (uint8_t i =0; i < KNX_TELEGRAM_MAX_SIZE; i++) _telegram[i] = 0;
  _controlField = CONTROL_FIELD_DEFAULT_VALUE ; _routing= ROUTING_FIELD_DEFAULT_VALUE;
}

   
void KnxTelegram::SetLongPayload(const uint8_t origin[], uint8_t nbOfBytes)
{
  if (nbOfBytes > KNX_TELEGRAM_PAYLOAD_MAX_SIZE-2) nbOfBytes = KNX_TELEGRAM_PAYLOAD_MAX_SIZE-2;
  for(uint8_t i=0; i < nbOfBytes; i++) _payloadChecksum[i] = origin[i];
}


void KnxTelegram::ClearLongPayload(void)
{
  for(uint8_t i=0; i < KNX_TELEGRAM_PAYLOAD_MAX_SIZE-1; i++) _payloadChecksum[i] = 0;
}


void KnxTelegram::GetLongPayload(uint8_t destination[], uint8_t nbOfBytes) const
{
  if (nbOfBytes > KNX_TELEGRAM_PAYLOAD_MAX_SIZE-2) nbOfBytes = KNX_TELEGRAM_PAYLOAD_MAX_SIZE-2 ;
  for(uint8_t i=0; i < nbOfBytes; i++) destination[i] = _payloadChecksum[i];
};
    

uint8_t KnxTelegram::CalculateChecksum(void) const
{
	uint8_t indexChecksum, xorSum=0;
  indexChecksum = KNX_TELEGRAM_HEADER_SIZE + GetPayloadLength() + 1;
  for (uint8_t i = 0; i < indexChecksum ; i++)   xorSum ^= _telegram[i]; // XOR Sum of all the databytes
  return (uint8_t)(~xorSum); // Checksum equals 1's complement of databytes XOR sum
}


void KnxTelegram::UpdateChecksum(void)
{
	uint8_t indexChecksum, xorSum=0;
  indexChecksum = KNX_TELEGRAM_HEADER_SIZE + GetPayloadLength() + 1;
  for (uint8_t i = 0; i < indexChecksum ; i++)   xorSum ^= _telegram[i]; // XOR Sum of all the databytes
  _telegram[indexChecksum] = ~xorSum; // Checksum equals 1's complement of databytes XOR sum
}


void KnxTelegram::Copy(KnxTelegram& dest) const
{
	uint8_t length = GetTelegramLength();
  for (uint8_t i=0; i<length ; i++)  dest._telegram[i] = _telegram[i];
}


void KnxTelegram::CopyHeader(KnxTelegram& dest) const
{
  for(uint8_t i=0; i < KNX_TELEGRAM_HEADER_SIZE; i++) dest._telegram[i] = _telegram[i];
}


e_KnxTelegramValidity KnxTelegram::GetValidity(void) const
{
  if ((_controlField & CONTROL_FIELD_PATTERN_MASK) != CONTROL_FIELD_VALID_PATTERN) return KNX_TELEGRAM_INVALID_CONTROL_FIELD; 
  if ((_controlField & CONTROL_FIELD_FRAME_FORMAT_MASK) != CONTROL_FIELD_STANDARD_FRAME_FORMAT) return KNX_TELEGRAM_UNSUPPORTED_FRAME_FORMAT; 
  if (!GetPayloadLength()) return KNX_TELEGRAM_INCORRECT_PAYLOAD_LENGTH ;
  if ((_commandH & COMMAND_FIELD_PATTERN_MASK) != COMMAND_FIELD_VALID_PATTERN) return KNX_TELEGRAM_INVALID_COMMAND_FIELD;
  if ( GetChecksum() != CalculateChecksum()) return KNX_TELEGRAM_INCORRECT_CHECKSUM ;
  uint8_t cmd=GetCommand();
  if  (    (cmd!=KNX_COMMAND_VALUE_READ) && (cmd!=KNX_COMMAND_VALUE_RESPONSE) 
       && (cmd!=KNX_COMMAND_VALUE_WRITE) && (cmd!=KNX_COMMAND_MEMORY_WRITE)) return KNX_TELEGRAM_UNKNOWN_COMMAND;
  return  KNX_TELEGRAM_VALID;
};


void KnxTelegram::Info( std::string& str ) const
{
	uint8_t payloadLength = GetPayloadLength();
	std::stringstream stream;

	stream << "SrcAddr=" << std::hex << GetSourceAddress();
	stream << "\nTargetAddr=" << std::hex << GetTargetAddress();
	stream << "\nPayloadLgth=" << payloadLength;
	stream << "\nCommand=";

	switch (GetCommand()) {
	case KNX_COMMAND_VALUE_READ:
		stream << "VAL_READ";
		break;

	case KNX_COMMAND_VALUE_RESPONSE:
		stream << "VAL_RESP";
		break;

	case KNX_COMMAND_VALUE_WRITE:
		stream << "VAL_WRITE";
		break;

	case KNX_COMMAND_MEMORY_WRITE:
		stream << "MEM_WRITE";
		break;

	default:
		stream << "ERR_VAL!";
		break;
	}
	stream << "\nPayload=" << std::hex << GetFirstPayloadByte() << ' ';
	for( uint8_t i = 0; i < payloadLength - 1; i++ )
		stream << std::hex << _payloadChecksum[i] << ' ';
	stream << '\n';

	str += stream.str();
}


void KnxTelegram::KnxTelegram::InfoRaw( std::string& str ) const
{
	std::stringstream stream;
	for( uint8_t i = 0; i < KNX_TELEGRAM_MAX_SIZE; i++ )
		stream << std::hex << _telegram[i] << ' ';

	str += stream.str();
}


void KnxTelegram::InfoVerbose( std::string& str ) const
{
	std::stringstream stream;
	uint8_t payloadLength = GetPayloadLength();

	stream << "Repeat=" << (IsRepeated() ? "YES" : "NO");
	stream << "\nPrio=";
	switch(GetPriority())
	{
		case KNX_PRIORITY_SYSTEM_VALUE : stream << "SYSTEM"; break;
		case KNX_PRIORITY_ALARM_VALUE : stream << "ALARM"; break;
		case KNX_PRIORITY_HIGH_VALUE : stream << "HIGH"; break;
		case KNX_PRIORITY_NORMAL_VALUE : stream << "NORMAL"; break;
		default: stream << "ERR_VAL!"; break;
	}
	stream << "\nSrcAddr=" << std::hex << GetSourceAddress();
	stream << "\nTargetAddr=" << std::hex << GetTargetAddress();
	stream << "\nGroupAddr=" << (IsMulticast() ? "YES" : "NO");
	stream << "\nRout.Counter=" << GetRoutingCounter();
	stream << "\nPayloadLgth=" << payloadLength;
	stream << "\nTelegramLength=" << GetTelegramLength();
	stream << "\nCommand=";
	switch( GetCommand() )
	{
		case KNX_COMMAND_VALUE_READ : stream << "VAL_READ"; break;
		case KNX_COMMAND_VALUE_RESPONSE : stream << "VAL_RESP"; break;
		case KNX_COMMAND_VALUE_WRITE : stream << "VAL_WRITE"; break;
		case KNX_COMMAND_MEMORY_WRITE : stream << "MEM_WRITE"; break;
		default : stream << "ERR_VAL!"; break;
	}

	stream << "\nPayload=" << std::hex << GetFirstPayloadByte();
	for ( uint8_t i = 0; i < payloadLength-1; i++ )
		stream << std::hex << _payloadChecksum[i] << ' ';

	stream << "\nValidity=";
	switch(GetValidity())
	{
		case KNX_TELEGRAM_VALID : stream << "VALID"; break;
		case KNX_TELEGRAM_INVALID_CONTROL_FIELD : stream << "INVALID_CTRL_FIELD"; break;
		case KNX_TELEGRAM_UNSUPPORTED_FRAME_FORMAT : stream << "UNSUPPORTED_FRAME_FORMAT"; break;
		case KNX_TELEGRAM_INCORRECT_PAYLOAD_LENGTH : stream << "INCORRECT_PAYLOAD_LGTH"; break;
		case KNX_TELEGRAM_INVALID_COMMAND_FIELD : stream << "INVALID_CMD_FIELD"; break;
		case KNX_TELEGRAM_UNKNOWN_COMMAND : stream << "UNKNOWN_CMD"; break;
		case KNX_TELEGRAM_INCORRECT_CHECKSUM : stream << "INCORRECT_CHKSUM"; break;
		default : stream << "ERR_VAL!"; break;
	}
	stream << '\n';

	str += stream.str();
}

// EOF
