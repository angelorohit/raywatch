
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

#ifndef SPHERE_HEADER
#define SPHERE_HEADER

#include "Primitive.h"

class Sphere : public Primitive
{
// Members
private:
    Vector<float>   _centre;
    float           _radius;

    // Auxiliaries
    float           _oneOverRadius;

public:
// Constructor
    explicit Sphere();
// Destructor
    virtual ~Sphere();

// Functions
private:
    void GetPolarCoordinatesAt(
        const Vector<float> &position,
        float &longitude,
        float &latitude ) const;

public:
    void SetCentre(const Vector<float> &centre);
    void SetRadius(const float &radius);

    // Primitive's functions
    virtual const bool Intersects(const Ray &ray, IntersectionInfo &intersectionInfo) const;
    virtual const bool Intersects(const Ray &ray, float &intersectionDist) const;
    virtual const Vector<float> GetSurfaceNormal(const Vector<float> &position) const;

    // Serializable's functions
    virtual const bool Read(Deserializer &d, void *const pUserData);
    virtual const bool Write(Serializer &s) const;
};

#endif
