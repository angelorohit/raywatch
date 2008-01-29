
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

#include "AreaLight.h"
#include "Ray.h"
#include "Scene.h"
#include "Maths.h"

// Constructor
AreaLight::AreaLight() :
    _positions()
{
}

// Destructor
AreaLight::~AreaLight()
{
}

// Functions

void AreaLight::SetRectangularArea(
    const Vector<float> &v1,
    const Vector<float> &v2,
    const Vector<float> &v3,
    const int           &numHorizontalSamples,
    const int           &numVerticalSamples )
{
    Vector<float> nx = v3 - v2;
    const float width = nx.Normalize();

    Vector<float> ny = v2 - v1;
    const float height = ny.Normalize();

    // Clear any existing positions
    _positions.clear();

    // Create the positions (with some randomness for lesser banding)
    for(int y=0; y < numVerticalSamples; ++y)
    for(int x=0; x < numHorizontalSamples; ++x)
    {
        _positions.push_back(
            v1 +
            nx * ((x + 0.25f + Maths::GenerateRandomValue() * 0.5f) * width / numHorizontalSamples) +
            ny * ((y + 0.25f + Maths::GenerateRandomValue() * 0.5f) * height / numVerticalSamples) );
    }
}

void AreaLight::SetSphericalArea(
    const Vector<float> &centre,
    const float         &radius,
    const int           &numSamples )
{
    // Clear any existing positions
    _positions.clear();

    // Create the positions along the surface of the sphere (randomly)
    for(int i=0; i < numSamples; ++i)
    {
        Vector<float> rndVec(
            Maths::GenerateRandomValue() - 0.5f,
            Maths::GenerateRandomValue() - 0.5f,
            Maths::GenerateRandomValue() - 0.5f );
        rndVec.Normalize();

        _positions.push_back( centre + rndVec * radius );
    }
}

// Light's Functions
void AreaLight::AccumulateIlluminationAtSurface(
    const Ray           &ray,
    const Vector<float> &surfaceNormal,
    const float         &surfaceRoughness,
    const Scene         &scene,
    Color &diffuse,
    Color &specular ) const
{
    if( _positions.empty() )
        return;

    Color areaLightDiffuse( 0 );
    Color areaLightSpecular( 0 );

    // Accumulate the illumination from all the positions
    for(VectorList::const_iterator itr = _positions.begin(); itr != _positions.end(); ++itr )
    {
        const Vector<float> &lightPosition = (*itr);

        Vector<float> lightRayDirection = lightPosition - ray.Origin();
        const float lightRayLength = lightRayDirection.Normalize();

        // If the light is out of range of the surface
        if( lightRayLength > _range )
            continue;

        // The illumination from this light
        const Ray lightRay( ray.Origin(), lightRayDirection, ray );
        const Color illuminationFromLight = Illumination( lightRay, lightRayLength, scene );

        if( illuminationFromLight.Magnitude2() < Maths::Tolerance )
            continue;

        // Calculate the illumination at the point on the surface
        const Color illumination = illuminationFromLight * ( 1 - lightRayLength * _oneOverRange );

        // Accumulate the diffuse
        areaLightDiffuse += illumination * Maths::Max<float>(0, surfaceNormal.Dot( lightRayDirection ));

        // Accumulate the specular
        areaLightSpecular += illuminationFromLight * powf( Maths::Max<float>(0, ray.Direction().Dot( lightRayDirection.Reflect( surfaceNormal ) ) ), surfaceRoughness );
    }

    diffuse += areaLightDiffuse * (1.0f / _positions.size());
    specular += areaLightSpecular * (1.0f / _positions.size());
}
