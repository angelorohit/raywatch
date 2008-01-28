
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

#ifndef RAYTRACER_HEADER
#define RAYTRACER_HEADER

#include "Color.h"
#include "Camera.h"

// Forward Declarations
class Ray;
class Scene;
class Image;

class RayTracer
{
// Members
private:

public:
// Constructor
    explicit RayTracer();
// Destructor
    ~RayTracer();

private:
// Copy Constructor / Assignment Operator
    RayTracer(const RayTracer &);
    const RayTracer &operator =(const RayTracer &);

// Functions
public:
    static const Color GetIllumination(const Ray &ray, const Scene &scene);

    const bool Render(const Camera &camera, const Scene &scene, Image &image) const;
};

#endif
