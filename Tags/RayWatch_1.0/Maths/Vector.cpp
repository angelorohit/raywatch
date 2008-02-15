
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

#include "Vector.h"
#include "Maths.h"
#include <math.h>

// Specialized template code

// Specializations for float type

template <>
const float Vector<float>::Magnitude() const
{
    return sqrt( Magnitude2() );
}

template <>
const float Vector<float>::Normalize()
{
    const float d2 = Magnitude2();
    if( d2 < Maths::Tolerance )
        return 0.0f;

    const float d = sqrt( d2 );
    const float oneOverD = 1.0f / d;

    x *= oneOverD;
    y *= oneOverD;
    z *= oneOverD;

    return d;
}
