
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

#ifndef TRIANGLE_HEADER
#define TRIANGLE_HEADER

#include "Primitive.h"

class Triangle : public Primitive
{
// Members
private:
    Vector<float>   _v1;
    Vector<float>   _v2;
    Vector<float>   _v3;

    // Auxiliaries
    Vector<float>   _surfaceNormal;
    Vector<float>   _edge1Normal;
    Vector<float>   _edge2Normal;
    Vector<float>   _edge3Normal;

public:
// Constructor
    explicit Triangle();
// Destructor
    virtual ~Triangle();

// Functions
public:
    // Primitive's functions
    virtual const bool Intersects(const Ray &ray, IntersectionInfo &intersectionInfo) const;
    virtual const bool Intersects(const Ray &ray, float &intersectionDist) const;
    virtual const Vector<float> GetSurfaceNormal(const Vector<float> &position) const;

    void SetVertices(const Vector<float> &v1, const Vector<float> &v2, const Vector<float> &v3);
};

#endif
