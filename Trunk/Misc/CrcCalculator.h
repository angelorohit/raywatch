
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

#ifndef CRCCALCULATOR_HEADER
#define CRCCALCULATOR_HEADER

class CrcCalculator
{
// Types
public:
    typedef unsigned long CrcType;
    typedef unsigned char Byte;

// Members
private:
    CrcType _register;

public:
// Constructor / Destructor
    explicit CrcCalculator();
    ~CrcCalculator();

private:
// Copy Constructor / Assignment Operator
    CrcCalculator(const CrcCalculator &);
    const CrcCalculator &operator =(const CrcCalculator &);

// Functions
public:
    void Begin();
    void Add(const Byte byte);
    const CrcType End();
};

#endif
