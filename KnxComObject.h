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


// File : KnxComObject.h
// Author : Franck Marini
// Description : Handling of the KNX Communication Objects
// Module dependencies : KnxTelegram

// Modified:
// 2019-10, JWJ

#ifndef KNXCOMOBJECT_H
#define KNXCOMOBJECT_H

#include "KnxTelegram.h"
#include "KnxDPT.h"

#include <string>

// !!!!!!!!!!!!!!! FLAG OPTIONS !!!!!!!!!!!!!!!!!
// By default, all the objects have NORMAL priority, other priorities are not supported
// turn KNX_COM_OBJ_SUPPORT_ALL_PRIORITIES flag on to allow support of all the priorities
// #define KNX_COM_OBJ_SUPPORT_ALL_PRIORITIES

// Definition of com obj indicator values
// See "knx.org" for com obj indicators specification
// INDICATOR field : B7  B6  B5  B4  B3  B2  B1  B0
//                   xx  xx   C   R   W   T   U   I  
#define KNX_COM_OBJ_C_INDICATOR	0x20 // Communication (C)
#define KNX_COM_OBJ_R_INDICATOR	0x10 // Read (R)
#define KNX_COM_OBJ_W_INDICATOR	0x08 // Write (W)
#define KNX_COM_OBJ_T_INDICATOR	0x04 // Transmit (T)
#define KNX_COM_OBJ_U_INDICATOR	0x02 // Update (U)
#define KNX_COM_OBJ_I_INDICATOR	0x01 // Init Read (I)

// Definition of predefined com obj profiles
// Sensor profile : COM_OBJ_SENSOR
#define COM_OBJ_SENSOR KNX_COM_OBJ_C_R_T_INDICATOR
#define KNX_COM_OBJ_C_R_T_INDICATOR 0x34 // ( Communication | Read | Transmit )
// Logic input profile : COM_OBJ_LOGIC_IN
#define COM_OBJ_LOGIC_IN KNX_COM_OBJ_C_W_U_INDICATOR
#define KNX_COM_OBJ_C_W_U_INDICATOR 0x2A  // ( Communication | Write | Update )
// Logic input to be initialized at bus power-up profile : COM_OBJ_LOGIC_IN_INIT
#define COM_OBJ_LOGIC_IN_INIT KNX_COM_OBJ_C_W_U_I_INDICATOR
#define KNX_COM_OBJ_C_W_U_I_INDICATOR 0x2B  // ( Communication | Write | Update | Init)

#define KNX_COM_OBJECT_OK       0
#define KNX_COM_OBJECT_ERROR    255


class KnxComObject {
	const uint16_t _addr; // Group Address value

	const uint8_t _dptId; // Datapoint type

	const uint8_t _indicator; // C/R/W/T/U/I indicators

	// Com object data length is calculated in the same way as telegram payload length
	// (See "knx.org" telegram specification for more details)
	const uint8_t _length;

#ifdef KNX_COM_OBJ_SUPPORT_ALL_PRIORITIES
	const e_KnxPriority _prio; // priority
#endif

	// _validity is used for "InitRead" typed com objs :
	// it remains "false" till the object value is updated
	// NB : the objects not typed "InitRead" get "true" validity value
	bool _validity;

	union {
		// field used in case of short value (1 byte max width, i.e. length <= 2)
		struct{
			uint8_t _value;
			uint8_t _notUSed;
		};
		// field used in case of long value (2 bytes width or more, i.e. length > 2)
		// The data space is allocated dynamically by the constructor
		uint8_t *_longValue;
	};
	
public:
  // Constructor :
#ifdef KNX_COM_OBJ_SUPPORT_ALL_PRIORITIES	
	KnxComObject(uint16_t addr, e_KnxDPT_ID dptId, e_KnxPriority prio, uint8_t indicator );
#else
	KnxComObject(uint16_t addr, e_KnxDPT_ID dptId, uint8_t indicator );
#endif
  // Destructor
	~KnxComObject();

  // INLINED functions (see definitions later in this file)
	uint16_t GetAddr(void) const;

	uint8_t GetDptId(void) const;

	e_KnxPriority GetPriority(void) const;

	uint8_t GetIndicator(void) const;

	bool GetValidity(void) const;

	uint8_t GetLength(void) const;

	// Return the com obj value (short value case only)
	uint8_t GetValue(void) const;

	// Update the com obj value (short value case only)
	// Return ERROR if the com obj is long value (invalid use case), else return OK
	uint8_t UpdateValue(uint8_t newVal);

	// Toggle the binary value (for com objs with "B1" format)
	// NB : the function does not change the validity.
	void ToggleValue(void);

  // functions NOT INLINED :

	// Get the com obj value (short and long value cases)
	void GetValue(uint8_t dest[]) const;

	// Update the com obj value (short and long value cases)
	void UpdateValue(const uint8_t ori[]);

	// Update the com obj value with a telegram payload content
	// Return ERROR if the telegram payload length differs from com obj one, else return OK
	uint8_t UpdateValue(const KnxTelegram& ori);

	// Copy the com obj attributes (addr, prio, length) into a telegram object
	void CopyAttributes(KnxTelegram& dest) const;

	// Copy the com obj value into a telegram object
	void CopyValue(KnxTelegram& dest) const;

	// DEBUG function
	void Info(std::string&) const;
};


// --------------- Definition of the INLINED functions -----------------
inline uint16_t KnxComObject::GetAddr(void) const { return _addr; }

inline uint8_t KnxComObject::GetDptId(void) const { return _dptId; }

#ifdef KNX_COM_OBJ_SUPPORT_ALL_PRIORITIES	
inline e_KnxPriority KnxComObject::GetPriority(void) const { return _prio; }
#else
inline e_KnxPriority KnxComObject::GetPriority(void) const { return KNX_PRIORITY_NORMAL_VALUE; }
#endif

inline uint8_t KnxComObject::GetIndicator(void) const { return _indicator; }

inline bool KnxComObject::GetValidity(void) const { return _validity; }

inline uint8_t KnxComObject::GetLength(void) const { return _length; }

inline uint8_t KnxComObject::GetValue(void) const { return _value; }

inline uint8_t KnxComObject::UpdateValue(uint8_t newValue)
{ if (_length > 2) return KNX_COM_OBJECT_ERROR; _value = newValue; _validity = true; return KNX_COM_OBJECT_OK; }

inline void KnxComObject::ToggleValue(void) { _value =  !_value; }

#endif // KNXCOMOBJECT_H
