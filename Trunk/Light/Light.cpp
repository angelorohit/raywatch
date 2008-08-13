
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

#include "Light.h"
#include "RayTracer.h"
#include "Scene.h"
#include "Deserializer.h"
#include "DeserializerHelper.h"
#include "SerializerHelper.h"

// Constructor
Light::Light() :
    _color( 1 ),
    _intensity( 1 ),
    _illumination( 1 ),
    _range( 0 ),
    _oneOverRange( 0 )
{
}

// Destructor
Light::~Light()
{
}

// Functions

const Color Light::Illumination(const Ray &lightRay, const float &lightRayLength, const Scene &scene ) const
{
    if( RayTracer::_bRayTraceShadows )
        return RayTracer::GetIllumination( lightRay, scene );

    if( scene.IsOccluded( lightRay, lightRayLength ) )
        return Color( 0 );

    return _illumination;
}

// Accessors
const Color &Light::Illumination() const
{
    return _illumination;
}

void Light::SetColor(const Color &color)
{
    _color = color;
    _illumination = _color * _intensity;
}

void Light::SetIntensity(const float &intensity)
{
    _intensity = intensity;
    _illumination = _color * _intensity;
}

void Light::SetRange(const float &range)
{
    _range = range;
    _oneOverRange = 1 / _range;
}

// Serializable's functions
const bool Light::Read(Deserializer &d)
{
    DESERIALIZE_CLASS( object, d, Light )
    {
        // Read the base
        if( !Serializable::Read( d ) )
            break;

        Color   color;
        float   intensity;
        float   range;

        if( !d.ReadObject( "color", color, Color(1) )   ||
            !d.ReadObject( "intensity", intensity, 1 )  ||
            !d.ReadObject( "range", range, 1 )          )
            break;

        SetColor( color );
        SetIntensity( intensity );
        SetRange( range );
    }

    return object.ReadResult();
}

const bool Light::Write(Serializer &s) const
{
    SERIALIZE_CLASS( object, s, Light )
    {
        // Write the base
        if( !Serializable::Write( s ) )
            break;

        if( !s.WriteObject( "color", _color, Color(1) )     ||
            !s.WriteObject( "intensity", _intensity, 1 )    ||
            !s.WriteObject( "range", _range, 1 )            )
            break;
    }

    return object.WriteResult();
}
