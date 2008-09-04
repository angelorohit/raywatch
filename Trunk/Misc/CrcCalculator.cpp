
//  RayWatch - A simple cross-platform RayTracer.
//  Copyright (C) 2008  Francis Xavier Joseph Pulikotil
//
//  This program is free software: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation, either version 3 of the License, or
//  (at your option) any later version.
//
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with this program.  If not, see <http://www.gnu.org/licenses/>.

#include "CrcCalculator.h"

// Constructor
CrcCalculator::CrcCalculator() :
    _register( 0xFFFFFFFF ) // Standard initial value in CRC32
{
}

// Destructor
CrcCalculator::~CrcCalculator()
{
}

// Functions
void CrcCalculator::Begin()
{
    _register = 0xFFFFFFFF; // Standard initial value in CRC32
}

void CrcCalculator::Add(const Byte byte)
{
	// The standard polynomial in CRC32
    const CrcType poly = 0xEDB88320;

    _register ^= byte;
    for(int i=0; i < 8; ++i)
    {
        if( _register & 0x1 )
            _register = (_register >> 1) ^ poly;
        else
            _register >>= 1;
    }
}

const CrcCalculator::CrcType CrcCalculator::End()
{
    return (_register ^ 0xFFFFFFFF);
}
