
//  RayWatch - A simple cross-platform RayTracer.
//  Copyright (C) 2008
//      Angelo Rohit Joseph Pulikotil,
//      Francis Xavier Joseph Pulikotil
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

#ifndef POINTLIGHT_HEADER
#define POINTLIGHT_HEADER

#include "Light.h"

class PointLight : public Light
{
// Members
public:
    Vector<float>   _position;

public:
// Constructor
    explicit PointLight();
// Destructor
    virtual ~PointLight();

// Functions
public:
    // Light's Functions
    virtual void AccumulateIlluminationAtSurface(
        const Ray           &ray,
        const Vector<float> &surfaceNormal,
        const float         &surfaceRoughness,
        const Scene         &scene,
        Color &diffuse,
        Color &specular ) const;

    // Serializable's functions
    virtual const bool Read(std::istream &stream);
    virtual const bool Write(std::ostream &stream) const;
};

#endif
