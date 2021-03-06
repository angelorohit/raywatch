
//  RayWatch - A simple cross-platform RayTracer.
//  Copyright (C) 2008  Angelo Rohit Joseph Pulikotil
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

#ifndef INTERSECTIONINFO_HEADER
#define INTERSECTIONINFO_HEADER

#include "Vector.h"

struct IntersectionInfo
{
    float           _dist;      // Distance at which the intersection occurs
    Vector<float>   _point;     // The intersection point
    float           _tU;        // The U texture coordinate at the intersection point
    float           _tV;        // The V texture coordinate at the intersection point
    bool            _bOnEntry;  // Whether the ray is entering the primitive or exiting it
};

#endif
