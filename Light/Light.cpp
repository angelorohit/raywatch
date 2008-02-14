
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
const bool Light::Read(std::istream &stream)
{
    // Read the base
    if( !ReadObjectHeader( stream, "Serializable" ) || !Serializable::Read( stream ) )
        return false;

    Color   color;
    float   intensity;
    float   range;

    if( !ReadVariable( stream, "color", color )         ||
        !ReadVariable( stream, "intensity", intensity ) ||
        !ReadVariable( stream, "range", range )         )
        return false;

    SetColor( color );
    SetIntensity( intensity );
    SetRange( range );

    return true;
}

const bool Light::Write(std::ostream &stream) const
{
    // Write the header
    if( !WriteVariable( stream, "object", "Light" ) )
        return false;

    Indent();
    {
        // Write the base
        if( !Serializable::Write( stream ) )
            return false;

        if( !WriteVariable( stream, "color", _color )           ||
            !WriteVariable( stream, "intensity", _intensity )   ||
            !WriteVariable( stream, "range", _range )           )
            return false;
    }
    Unindent();

    return true;
}
