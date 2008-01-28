
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

#ifndef LIGHT_HEADER
#define LIGHT_HEADER

#include "Color.h"

// Forward Declarations
class Ray;
class Scene;

class Light
{
// Members
protected:
    Color   _color;
    float   _intensity;
    float   _range;

    // Auxiliaries
    Color   _illumination;  // _color * _intensity;
    float   _oneOverRange;

public:
// Constructor
    explicit Light();
// Destructor
    virtual ~Light();

// Functions
public:
    // Accessors
    const Color &Illumination() const;

    void SetColor(const Color &color);
    void SetIntensity(const float &intensity);
    void SetRange(const float &range);

    virtual void AccumulateIlluminationAtSurface(
        const Ray           &ray,
        const Vector<float> &surfaceNormal,
        const float         &surfaceRoughness,
        const Scene         &scene,
        Color &diffuse,
        Color &specular ) const = 0;
};

#endif
