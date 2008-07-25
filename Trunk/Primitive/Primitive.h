
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

#ifndef PRIMITIVE_HEADER
#define PRIMITIVE_HEADER

#include "Material.h"
#include "IntersectionInfo.h"
#include "Serializable.h"

// Forward Declarations
class Ray;
class Light;

class Primitive : public Serializable
{
// Members
public:
    Material        _material;
    const Light    *_pLight;

protected:
// Constructor
    explicit Primitive();
public:
// Destructor
    virtual ~Primitive();

// Functions
public:
    virtual const bool Intersects(const Ray &ray, IntersectionInfo &intersectionInfo) const = 0;
    virtual const bool Intersects(const Ray &ray, float &intersectionDist) const = 0;
    virtual const Vector<float> GetSurfaceNormal(const Vector<float> &position) const = 0;

    // Serializable's functions
    virtual const bool Read(Deserializer &d);
    virtual const bool Write(Serializer &s) const;
    virtual const bool RestorePointers(AddressTranslator &t);
};

#endif
