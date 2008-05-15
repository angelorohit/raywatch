
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
#include "Texture.h"
#include "SafeDelete.h"
#include "Maths.h"
#include "ObjectFactory.h"
#include "Deserializer.h"
#include "DeserializerHelper.h"
#include "SerializerHelper.h"
#include <algorithm>
#include <limits>

// Register with the ObjectFactory
ObjectFactory_Register(Serializable, Scene);

// Constructor
Scene::Scene() :
    _primitiveList(),
    _lightList(),
    _textureList(),
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

    // Delete all the Textures
    for(TextureList::iterator itr = _textureList.begin(); itr != _textureList.end(); ++itr)
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

void Scene::AddTexture(Texture *const pTexture)
{
    if( !pTexture )
        return;

    // If the Texture is already in the list, then don't do anything.
    if( std::find(_textureList.begin(), _textureList.end(), pTexture) != _textureList.end() )
        return;

    _textureList.push_back( pTexture );
}

void Scene::RemoveTexture(Texture *const pTexture)
{
    _textureList.remove( pTexture );
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

Texture *const Scene::LoadTexture(const std::string &fileName)
{
    // See if this texture is already in our list.
    for(TextureList::const_iterator itr = _textureList.begin(); itr != _textureList.end(); ++itr)
    {
        Texture *const pTexture = (*itr);

        if( pTexture->FileName().compare( fileName ) == 0 )
            return pTexture;
    }

    // It's not, so we'll have to create it.
    Texture *pTexture = new Texture();
    if( !pTexture )
        return 0;

    // Load the texture
    if( !pTexture->Load( fileName ) )
    {
        SAFE_DELETE_SCALAR( pTexture );
        return 0;
    }

    // Push it into our list of textures
    AddTexture( pTexture );

    return pTexture;
}

// Serializable's functions
const bool Scene::Read(std::istream &stream)
{
    DESERIALIZE_CLASS( object, stream, Scene )
    {
        // Read the base
        if( !Serializable::Read( stream ) )
            break;

        if( !Deserializer::ReadVariable( stream, "ambientLight", _ambientLight )            ||
            !Deserializer::ReadVariable( stream, "maxRayGenerations", _maxRayGenerations )  )
            break;

        // Read the children
        DESERIALIZE_LIST( children, stream, "Children" )
        {
            // Peek the next object header
            std::string objectType;
            if( !Deserializer::PeekHeader( stream, objectType ) )
                break;

            // Create the object
            Serializable *pSerializable = ObjectFactory<Serializable>::Create( objectType );
            if( !pSerializable )
            {
                std::cout << "Error: Unknown Object found: " << objectType << std::endl;
                break;
            }

            // Load the object
            if( !pSerializable->Read( stream ) )
            {
                SAFE_DELETE_SCALAR( pSerializable );
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

                // See if it's a Texture
                Texture *pTexture = dynamic_cast<Texture *>(pSerializable);
                if( pTexture )
                {
                    AddTexture( pTexture );
                    continue;
                }

                // We don't know what type of object this is
                {
                    std::cout << "Error: Object '" << objectType << "' cannot be inserted directly into the Scene." << std::endl;
                    SAFE_DELETE_SCALAR( pSerializable );
                    break;
                }
            }
        }

        if( children.ReadFailed() )
            break;
    }

    return object.ReadResult();
}

const bool Scene::Write(std::ostream &stream) const
{
    SERIALIZE_CLASS( object, stream, Scene )
    {
        // Write the base
        if( !Serializable::Write( stream ) )
            break;

        if( !Serializer::WriteVariable( stream, "ambientLight", _ambientLight )             ||
            !Serializer::WriteVariable( stream, "maxRayGenerations", _maxRayGenerations )   )
            break;

        // Write all the children
        SERIALIZE_OBJECT( children, stream, "Children" )
        {
            // Write all the Primitives
            {
                PrimitiveList::const_iterator itr;
                for(itr = _primitiveList.begin(); itr != _primitiveList.end(); ++itr)
                {
                    if( !(*itr)->Write( stream ) )
                        break;
                }
                if( itr != _primitiveList.end() )
                    break;
            }

            // Write all the Lights
            {
                LightList::const_iterator itr;
                for(itr = _lightList.begin(); itr != _lightList.end(); ++itr)
                {
                    if( !(*itr)->Write( stream ) )
                        break;
                }
                if( itr != _lightList.end() )
                    break;
            }

            // Write all the Textures
            {
                TextureList::const_iterator itr;
                for(itr = _textureList.begin(); itr != _textureList.end(); ++itr)
                {
                    if( !(*itr)->Write( stream ) )
                        break;
                }
                if( itr != _textureList.end() )
                    break;
            }
        }

        if( children.WriteFailed() )
            break;
    }

    return object.WriteResult();
}
