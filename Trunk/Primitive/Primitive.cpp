
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
#include "DeserializerHelper.h"
#include "SerializerHelper.h"

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
const bool Primitive::Read(Deserializer &d, void *const /*pUserData*/)
{
    DESERIALIZE_CLASS( object, d, Primitive )
    {
        // Read the base
        if( !Serializable::Read( d, 0 ) )
            break;

        if( !d.ReadObject( "material", _material, 0 )                           ||
            !d.ReadObject( "light", _pLight, DefaultValue<const Light *>(0) )   )
            break;
    }

    return object.ReadResult();
}

const bool Primitive::Write(Serializer &s) const
{
    SERIALIZE_CLASS( object, s, Primitive )
    {
        // Write the base
        if( !Serializable::Write( s ) )
            break;

        if( !s.WriteObject( "material", _material )                             ||
            !s.WriteObject( "light", _pLight, DefaultValue<const Light *>(0) )  )
            break;
    }

    return object.WriteResult();
}

const bool Primitive::RestorePointers(AddressTranslator &t)
{
    // Restore the base
    if( !Serializable::RestorePointers( t ) )
        return false;

    if( !t.TranslateAddress( _pLight ) )
        return false;

    return true;
}
