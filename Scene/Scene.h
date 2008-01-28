
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

#ifndef SCENE_HEADER
#define SCENE_HEADER

#include "Color.h"
#include "IntersectionInfo.h"
#include <list>

// Forward Declarations
class Ray;
class Primitive;
class Light;

class Scene
{
// Typedefs
public:
    typedef std::list<Primitive *>  PrimitiveList;
    typedef std::list<Light *>      LightList;

// Members
private:
    PrimitiveList   _primitiveList;
    LightList       _lightList;

public:
    Color           _ambientLight;
    int             _maxRayGenerations;

public:
// Constructor
    explicit Scene();
// Destructor
    ~Scene();

private:
// Copy Constructor / Assignment Operator
    Scene(const Scene &);
    const Scene &operator =(const Scene &);

// Functions
public:
    void AddPrimitive(Primitive *const pPrimitive);
    void RemovePrimitive(Primitive *const pPrimitive);

    void AddLight(Light *const pLight);
    void RemoveLight(Light *const pLight);

    const Primitive *const FindClosestIntersection(const Ray &ray, IntersectionInfo &closestIntersectionInfo) const;
    const bool IsOccluded(const Ray &ray, const float &rayLength) const;
    void GetSurfaceIllumination(
        const Ray           &ray,
        const Vector<float> &surfaceNormal,
        const float         &surfaceRoughness,
        Color &diffuse,
        Color &specular ) const;
};

#endif
