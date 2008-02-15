
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

#include "Maths.h"
#include <stdlib.h>

namespace Maths
{
// Functions

    const bool IsApproxEqual(const float &val1, const float &val2)
    {
        return(
            (val1 < (val2 + Tolerance)) &&
            (val1 > (val2 - Tolerance))
        );
    }

    const float DegToRad(const float &angle)
    {
        return (angle / 180.0f) * Pi;
    }

    const float RadToDeg(const float &angle)
    {
        return (angle / Pi) * 180.0f;
    }

    const float GenerateRandomValue()
    {
        return (rand() / (float)RAND_MAX);
    }

} // namespace Maths
