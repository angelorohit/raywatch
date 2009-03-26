
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

#ifndef AREALIGHT_HEADER
#define AREALIGHT_HEADER

#include "Light.h"
#include <vector>

class AreaLight : public Light
{
// Members
private:
    typedef std::vector<Vector<float> >  VectorList;
    VectorList  _positions;

    // For Serializing
    Vector<float>   _v1;
    Vector<float>   _v2;
    Vector<float>   _v3;
    int             _numHorizontalSamples;
    int             _numVerticalSamples;

public:
// Constructor
    explicit AreaLight();
// Destructor
    virtual ~AreaLight();

// Functions
public:
    void SetRectangularArea(
        const Vector<float> &v1,
        const Vector<float> &v2,
        const Vector<float> &v3,
        const int           &numHorizontalSamples,
        const int           &numVerticalSamples );

    // Note: This functionality should be moved into another class (could be SphericalAreaLight)
    //       Right now, it's not completely clear how to serialize two types of functionalities
    //void SetSphericalArea(
    //    const Vector<float> &centre,
    //    const float         &radius,
    //    const int           &numSamples );

    // Light's Functions
    virtual void AccumulateIlluminationAtSurface(
        const Ray           &ray,
        const Vector<float> &surfaceNormal,
        const float         &surfaceRoughness,
        const Scene         &scene,
        Color &diffuse,
        Color &specular ) const;

    // Serializable's functions
    virtual const bool Read(Deserializer &d, void *const pUserData);
    virtual const bool Write(Serializer &s) const;
};

#endif
