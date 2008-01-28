
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

#include "PointLight.h"
#include "Ray.h"
#include "Scene.h"
#include "Maths.h"

// Constructor
PointLight::PointLight() :
    _position( 0 )
{
}

// Destructor
PointLight::~PointLight()
{
}

// Functions

// Light's Functions
void PointLight::AccumulateIlluminationAtSurface(
    const Ray           &ray,
    const Vector<float> &surfaceNormal,
    const float         &surfaceRoughness,
    const Scene         &scene,
    Color &diffuse,
    Color &specular ) const
{
    Vector<float> lightRayDirection = _position - ray.Origin();
    const float lightRayLength = lightRayDirection.Normalize();

    // If the light is out of range of the surface
    if( lightRayLength > _range )
        return;

    // See if this light is occluded or not
    const Ray lightRay( ray.Origin(), lightRayDirection, Ray::RootGeneration() );   // Note: Use of RootGeneration() is allowed here because
                                                                                    // the temporary ray is being created only for intersection
                                                                                    // testing purposes, not for recursive ray-tracing.
    if( scene.IsOccluded(lightRay, lightRayLength) )
        return;

    // Calculate the illumination at the point on the surface
    const Color illumination = _illumination * ( 1 - lightRayLength * _oneOverRange );

    // Accumulate the diffuse
    diffuse += illumination * Maths::Max<float>(0, surfaceNormal.Dot( lightRayDirection ));

    // Accumulate the specular
    specular += _illumination * powf( Maths::Max<float>(0, ray.Direction().Dot( lightRayDirection.Reflect( surfaceNormal ) ) ), surfaceRoughness );
}
