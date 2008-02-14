
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

#include "Scene.h"
#include "Primitive.h"
#include "Light.h"
#include "SafeDelete.h"
#include "Maths.h"
#include "ObjectFactory.h"
#include <algorithm>
#include <limits>

// Register with the ObjectFactory
ObjectFactory_Register(Serializable, Scene);

// Constructor
Scene::Scene() :
    _primitiveList(),
    _lightList(),
    _ambientLight( 0 ),
    _maxRayGenerations( 3 )
{
}

// Destructor
Scene::~Scene()
{
    // Delete all the Primitives
    for(PrimitiveList::iterator itr = _primitiveList.begin(); itr != _primitiveList.end(); ++itr)
        SAFE_DELETE_SCALAR( *itr );

    // Delete all the Lights
    for(LightList::iterator itr = _lightList.begin(); itr != _lightList.end(); ++itr)
        SAFE_DELETE_SCALAR( *itr );
}

// Functions

void Scene::AddPrimitive(Primitive *const pPrimitive)
{
    if( !pPrimitive )
        return;

    // If the Primitive is already in the list, then don't do anything.
    if( std::find(_primitiveList.begin(), _primitiveList.end(), pPrimitive) != _primitiveList.end() )
        return;

    _primitiveList.push_back( pPrimitive );
}

void Scene::RemovePrimitive(Primitive *const pPrimitive)
{
    _primitiveList.remove( pPrimitive );
}

void Scene::AddLight(Light *const pLight)
{
    if( !pLight )
        return;

    // If the Light is already in the list, then don't do anything.
    if( std::find(_lightList.begin(), _lightList.end(), pLight) != _lightList.end() )
        return;

    _lightList.push_back( pLight );
}

void Scene::RemoveLight(Light *const pLight)
{
    _lightList.remove( pLight );
}

const Primitive *const Scene::FindClosestIntersection(const Ray &ray, IntersectionInfo &closestIntersectionInfo) const
{
    closestIntersectionInfo._dist = std::numeric_limits<float>::max();
    const Primitive *pClosestIntersectedPrimitive = 0;

    // Go through all the primitives and see which is the closest one which intersects
    for(PrimitiveList::const_iterator itr = _primitiveList.begin(); itr != _primitiveList.end(); ++itr)
    {
        const Primitive *const pPrimitive = *itr;

        IntersectionInfo intersectionInfo;
        if( pPrimitive->Intersects( ray, intersectionInfo ) && (intersectionInfo._dist < closestIntersectionInfo._dist) )
        {
            closestIntersectionInfo = intersectionInfo;
            pClosestIntersectedPrimitive    = pPrimitive;
        }
    }

    return pClosestIntersectedPrimitive;
}

const bool Scene::IsOccluded(const Ray &ray, const float &rayLength) const
{
    // Go through all the primitives (which are not light sources) and see if they intersect the ray
    for(PrimitiveList::const_iterator itr = _primitiveList.begin(); itr != _primitiveList.end(); ++itr)
    {
        const Primitive *const pPrimitive = *itr;

        if( pPrimitive->_pLight )
            continue;

        float intersectionDist;
        if( pPrimitive->Intersects( ray, intersectionDist ) && (intersectionDist < rayLength) )
            return true;
    }

    return false;
}

void Scene::GetSurfaceIllumination(
    const Ray           &ray,
    const Vector<float> &surfaceNormal,
    const float         &surfaceRoughness,
    Color &diffuse,
    Color &specular ) const
{
    // Initialize the returnables
    diffuse     = _ambientLight;
    specular    .Set( 0 );

    // Accumulate illumination from all the lights
    for(LightList::const_iterator itr = _lightList.begin(); itr != _lightList.end(); ++itr)
    {
        const Light *const pLight = *itr;

        // Accumulate the diffuse and specular contribution from this light
        pLight->AccumulateIlluminationAtSurface( ray, surfaceNormal, surfaceRoughness, *this, diffuse, specular );
    }
}

// Serializable's functions
const bool Scene::Read(std::istream &stream)
{
    // Read the base
    if( !ReadObjectHeader( stream, "Serializable" ) || !Serializable::Read( stream ) )
        return false;

    if( !ReadVariable( stream, "ambientLight", _ambientLight ) )
        return false;

    if( !ReadVariable( stream, "maxRayGenerations", _maxRayGenerations ) )
        return false;

    // Read all the objects in the scene
    bool bReadResult = true;
    for(;;)
    {
        // Read the next Object's type
        std::string objectType;
        if( !ReadVariable( stream, "object", objectType, true ) )
            break;

        // Create the object
        Serializable *pSerializable = ObjectFactory<Serializable>::Create( objectType );
        if( !pSerializable )
        {
            std::cout << "Error: Unknown Object found: " << objectType << std::endl;
            bReadResult = false;
            break;
        }

        // Load the object
        if( !pSerializable->Read( stream ) )
        {
            SAFE_DELETE_SCALAR( pSerializable );
            bReadResult = false;
            break;
        }

        // Depending on the type of object it is, push it into the appropriate list
        {
            // See if it's a Primitive
            Primitive *pPrimitive = dynamic_cast<Primitive *>(pSerializable);
            if( pPrimitive )
            {
                AddPrimitive( pPrimitive );
                continue;
            }

            // See if it's a Light
            Light *pLight = dynamic_cast<Light *>(pSerializable);
            if( pLight )
            {
                AddLight( pLight );
                continue;
            }

            // We don't know what type of object this is
            {
                std::cout << "Error: Object '" << objectType << "' is not a 'Primitive' or a 'Light'" << std::endl;
                SAFE_DELETE_SCALAR( pSerializable );
                bReadResult = false;
                break;
            }
        }
    } // for(;;)

    return bReadResult;
}

const bool Scene::Write(std::ostream &stream) const
{
    // Write the header
    if( !WriteVariable( stream, "object", "Scene" ) )
        return false;

    Indent();
    {
        // Write the base
        if( !Serializable::Write( stream ) )
            return false;

        if( !WriteVariable( stream, "ambientLight", _ambientLight ) )
            return false;

        if( !WriteVariable( stream, "maxRayGenerations", _maxRayGenerations ) )
            return false;

        // Write all the Primitives
        for(PrimitiveList::const_iterator itr = _primitiveList.begin(); itr != _primitiveList.end(); ++itr)
        {
            if( !(*itr)->Write( stream ) )
                return false;
        }

        // Write all the Lights
        for(LightList::const_iterator itr = _lightList.begin(); itr != _lightList.end(); ++itr)
        {
            if( !(*itr)->Write( stream ) )
                return false;
        }
    }
    Unindent();

    return true;
}
