
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

#ifndef AUTOCOUNTER_HEADER
#define AUTOCOUNTER_HEADER

#include <cassert>

class AutoCounter
{
// Members
private:
    int &_count;

public:
// Constructor
    explicit AutoCounter(int &count) :
        _count( count )
    {
        assert( _count >= 0 );
        ++_count;
    }

// Destructor
    ~AutoCounter()
    {
        --_count;
        assert( _count >= 0 );
    }

private:
// Copy Constructor / Assignment Operator
    AutoCounter(const AutoCounter &);
    const AutoCounter &operator =(const AutoCounter &);

// Functions
public:
    const bool IsRoot() const
    {
        assert( _count >= 1 );
        return (_count == 1);
    }
};

#endif
