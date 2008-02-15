
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

#include "Primitive.h"
#include "Light.h"
#include "Deserializer.h"

// Constructor
Primitive::Primitive() :
    _material(),
    _pLight( 0 )
{
}

// Destructor
Primitive::~Primitive()
{
}

// Serializable's functions
const bool Primitive::Read(std::istream &stream)
{
    // Read the base
    if( !ReadHeader( stream, "Serializable" ) || !Serializable::Read( stream ) )
        return false;

    // Read the material
    if( !ReadHeader( stream, "Material" ) || !_material.Read( stream ) )
        return false;

    // Read the light pointer
    if( !ReadVariable( stream, "light", _pLight ) )
        return false;

    if( !ReadFooter( stream, "Primitive" ) )
        return false;

    return true;
}

const bool Primitive::Write(std::ostream &stream) const
{
    if( !WriteHeader( stream, "Primitive" ) )
        return false;

    Indent();
    {
        // Write the base
        if( !Serializable::Write( stream ) )
            return false;

        if( !_material.Write( stream ) )
            return false;

        if( !WriteVariable( stream, "light", _pLight ) )
            return false;
    }
    Unindent();

    if( !WriteFooter( stream, "Primitive" ) )
        return false;

    return true;
}

const bool Primitive::RestorePointers()
{
    if( !Deserializer::TranslateAddress( _pLight ) )
        return false;

    return true;
}
