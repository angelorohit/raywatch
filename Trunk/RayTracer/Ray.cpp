
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

#include "Ray.h"

// Constructors
Ray::Ray(const int &generation) :
    _generation( generation )
{
}

Ray::Ray(const Vector<float> &origin, const Vector<float> &direction, const Ray &currentGeneration) :
    _origin( origin ),
    _direction( direction ),
    _generation( currentGeneration._generation + 1 )
{
}

// Functions

// Accessors
const Vector<float> &Ray::Origin() const
{
    return _origin;
}

const Vector<float> &Ray::Direction() const
{
    return _direction;
}

const int &Ray::Generation() const
{
    return _generation;
}

const Ray &Ray::RootGeneration()
{
    static Ray ray( 0 );
    return ray;
}
