
//  RayWatch - A simple cross-platform RayTracer.
//  Copyright (C) 2008  Francis Xavier Joseph Pulikotil
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

#include "Serializable.h"
#include "DeserializerHelper.h"
#include "SerializerHelper.h"
#include "Utility.h"

// Constructor
Serializable::Serializable() :
    _serializationDepth( 0 )
{
}

// Destructor
Serializable::~Serializable()
{
}

// Functions
const std::size_t Serializable::RandomAddress() const
{
    static std::size_t randomAddress = 0;
    ++randomAddress;
    return randomAddress;
}

const bool Serializable::Read(Deserializer &d)
{
    DESERIALIZE_CLASS( object, d, Serializable )
    {
        // Generate a random address
        Serializable *pDefaultOldThis = reinterpret_cast<Serializable*>( RandomAddress() );

        // Read the old address
        Serializable *pOldThis = 0;
        if( !d.ReadObject( "address", pOldThis, DefaultValue<Serializable*>( pDefaultOldThis ) ) )
            break;

        // Convert the old address to an unsigned-int
        const std::size_t oldAddress = reinterpret_cast<std::size_t>( pOldThis );

        // Register this Serializable's old address with its current one.
        if( !d.Register( oldAddress, this ) )
            break;
    }

    return object.ReadResult();
}

const bool Serializable::Write(Serializer &s) const
{
    SERIALIZE_CLASS( object, s, Serializable )
    {
        if( !s.WriteObject( "address", this ) )
            break;
    }

    return object.WriteResult();
}

const bool Serializable::RestorePointers(AddressTranslator &/*t*/)
{
    // Note: Empty default implementation; override to fix any required pointers
    return true;
}
