
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

// Constructor
Serializable::Serializable()
{
}

// Destructor
Serializable::~Serializable()
{
}

// Functions

const bool Serializable::Read(Deserializer &d)
{
    DESERIALIZE_CLASS( object, d, Serializable )
    {
        std::size_t oldAddress;
        if( !d.ReadObject( "address", oldAddress ) )
            break;

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

const bool Serializable::RestorePointers(Deserializer &/*d*/)
{
    // Note: Empty default implementation; override to fix any required pointers
    return true;
}
