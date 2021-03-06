
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

#include "AreaLight.h"
#include "Ray.h"
#include "Scene.h"
#include "Maths.h"
#include "ObjectFactory.h"
#include "Deserializer.h"
#include "DeserializerHelper.h"
#include "SerializerHelper.h"
#include "ForEach.h"

// Register with the ObjectFactory
ObjectFactory_Register(Serializable, AreaLight);

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
    _v1                     = v1;                   // Only for Serializing later
    _v2                     = v2;                   // Only for Serializing later
    _v3                     = v3;                   // Only for Serializing later
    _numHorizontalSamples   = numHorizontalSamples; // Only for Serializing later
    _numVerticalSamples     = numVerticalSamples;   // Only for Serializing later

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

// Note: This functionality should be moved into another class (could be SphericalAreaLight)
//       Right now, it's not completely clear how to serialize two types of functionalities
//void AreaLight::SetSphericalArea(
//    const Vector<float> &centre,
//    const float         &radius,
//    const int           &numSamples )
//{
//    // Clear any existing positions
//    _positions.clear();
//
//    // Create the positions along the surface of the sphere (randomly)
//    for(int i=0; i < numSamples; ++i)
//    {
//        Vector<float> rndVec(
//            Maths::GenerateRandomValue() - 0.5f,
//            Maths::GenerateRandomValue() - 0.5f,
//            Maths::GenerateRandomValue() - 0.5f );
//        rndVec.Normalize();
//
//        _positions.push_back( centre + rndVec * radius );
//    }
//}

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
    FOR_EACH( itr, VectorList, _positions )
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

// Serializable's functions
const bool AreaLight::Read(Deserializer &d, void *const /*pUserData*/)
{
    DESERIALIZE_CLASS( object, d, AreaLight )
    {
        // Read the base
        if( !Light::Read( d, 0 ) )
            break;

        Vector<float> vertex1, vertex2, vertex3;
        int numHorizontalSamples, numVerticalSamples;
        if( !d.ReadObject( "vertex1", vertex1 )                             ||
            !d.ReadObject( "vertex2", vertex2 )                             ||
            !d.ReadObject( "vertex3", vertex3 )                             ||
            !d.ReadObject( "numHorizontalSamples", numHorizontalSamples )   ||
            !d.ReadObject( "numVerticalSamples", numVerticalSamples )       )
            break;

        SetRectangularArea( vertex1, vertex2, vertex3, numHorizontalSamples, numVerticalSamples );
    }

    return object.ReadResult();
}

const bool AreaLight::Write(Serializer &s) const
{
    SERIALIZE_CLASS( object, s, AreaLight )
    {
        // Write the base
        if( !Light::Write( s ) )
            break;

        if( !s.WriteObject( "vertex1", _v1 )                                ||
            !s.WriteObject( "vertex2", _v2 )                                ||
            !s.WriteObject( "vertex3", _v3 )                                ||
            !s.WriteObject( "numHorizontalSamples", _numHorizontalSamples ) ||
            !s.WriteObject( "numVerticalSamples", _numVerticalSamples )     )
            break;
    }

    return object.WriteResult();
}
