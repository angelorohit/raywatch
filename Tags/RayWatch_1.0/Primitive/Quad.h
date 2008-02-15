
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

#ifndef QUAD_HEADER
#define QUAD_HEADER

#include "Primitive.h"

class Quad : public Primitive
{
// Members
private:
    Vector<float>   _topLeft;

    // Auxiliaries
    Vector<float>   _surfaceNormal;
    Vector<float>   _horizontalNormal;
    Vector<float>   _verticalNormal;
    float           _width;
    float           _height;

    // For Serializing
    Vector<float>   _v2;
    Vector<float>   _v3;

public:
// Constructor
    explicit Quad();
// Destructor
    virtual ~Quad();

// Functions
public:
    // Primitive's functions
    virtual const bool Intersects(const Ray &ray, IntersectionInfo &intersectionInfo) const;
    virtual const bool Intersects(const Ray &ray, float &intersectionDist) const;
    virtual const Vector<float> GetSurfaceNormal(const Vector<float> &position) const;

    void SetVertices(const Vector<float> &v1, const Vector<float> &v2, const Vector<float> &v3);

    // Serializable's functions
    virtual const bool Read(std::istream &stream);
    virtual const bool Write(std::ostream &stream) const;
};

#endif
