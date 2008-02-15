
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

#ifndef MATHS_HEADER
#define MATHS_HEADER

#include <math.h>

namespace Maths
{
// Constants

    const float Tolerance = 0.0001f;
    const float Pi        = 3.1415926535897932384626433832795f;

// Templates

    template <class T>
    const T Min(const T &val1, const T &val2)
    {
        return( (val2 < val1)? val2: val1 );
    }

    template <class T>
    const T Max(const T &val1, const T &val2)
    {
        return( (val2 > val1)? val2: val1 );
    }

    template <class T>
    const T Abs(const T &val)
    {
        return( (val < 0)? -val: val );
    }

    template <class T>
    const T Bound(const T &val, const T &min, const T &max)
    {
        if( val < min )
            return min;
        if( val > max )
            return max;
        return val;
    }

    // Interpolation functions
    template <class T>
    const T InterpolateLinear(const T &a, const T &b, const float &t)
    {
        return a + (b - a) * t;
    }

    template <class T>
    const T InterpolateCosine(const T &a, const T &b, const float &t)
    {
        const float u = (1.0f - cos(t * Pi)) * 0.5f;
        return a + (b - a) * u;
    }

    template <class T>
    const T InterpolateCubic(const T &a, const T &b, const T &c, const T &d, const float &t)
    {
        const float t2 = t * t;
        const float t3 = t2 * t;

        const T p0 = d - c - a + b;
        const T p1 = a - b - p0;
        const T p2 = c - a;
        const T p3 = b;

        return (p0*t3 + p1*t2 + p2*t + p3);
    }

    template <class T>
    const T InterpolateCubicDerivative(const T &a, const T &b, const T &c, const T &d, const float &t)
    {
        const float t2 = t * t;

        const T p0 = d - c - a + b;
        const T p1 = a - b - p0;
        const T p2 = c - a;

        return (p0*3*t2 + p1*2*t + p2);
    }

// Non-Templates

    const bool IsApproxEqual(const float &val1, const float &val2);
    
    const float DegToRad(const float &angle);   // Converts degrees to radians
    const float RadToDeg(const float &angle);   // Converts radians to degrees

    const float GenerateRandomValue();  // Returns a random value in the range 0.0f to 1.0f

} // namespace Maths

#endif
