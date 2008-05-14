
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
#include "Utility.h"
#include "Deserializer.h"
#include "DeserializerHelper.h"
#include "SerializerHelper.h"

// Initialize static members
int Serializable::_indentation( 0 );

// Constructor
Serializable::Serializable()
{
}

// Destructor
Serializable::~Serializable()
{
}

// Functions

void Serializable::Indent()
{
    _indentation += 4;
}

void Serializable::Unindent()
{
    _indentation -= 4;
}

const bool Serializable::WriteVariable(std::ostream &stream, const std::string &variable, const std::string &value)
{
    return (stream << std::string( _indentation, ' ' ) << variable << " = " << value << ";" << std::endl).good();
}

const bool Serializable::WriteVariable(std::ostream &stream, const std::string &variable, const char *const value)
{
    return WriteVariable( stream, variable, std::string( value ) );
}

const bool Serializable::WriteVariable(std::ostream &stream, const std::string &variable, const int &value)
{
    return WriteVariable( stream, variable, Utility::String::ToString( value ) );
}

const bool Serializable::WriteVariable(std::ostream &stream, const std::string &variable, const float &value)
{
    return WriteVariable( stream, variable, Utility::String::ToString( value ) );
}

const bool Serializable::WriteVariable(std::ostream &stream, const std::string &variable, const bool &value)
{
    const std::string strValue = value? "true": "false";
    return WriteVariable( stream, variable, strValue );
}

const bool Serializable::WriteVariable(std::ostream &stream, const std::string &variable, const Vector<int> &value)
{
    const std::string strValue =
        Utility::String::ToString(value.x) + ", " +
        Utility::String::ToString(value.y) + ", " +
        Utility::String::ToString(value.z);
    
    return WriteVariable( stream, variable, strValue );
}

const bool Serializable::WriteVariable(std::ostream &stream, const std::string &variable, const Vector<float> &value)
{
    const std::string strValue =
        Utility::String::ToString(value.x) + ", " +
        Utility::String::ToString(value.y) + ", " +
        Utility::String::ToString(value.z);
    
    return WriteVariable( stream, variable, strValue );
}

const bool Serializable::WriteVariable(std::ostream &stream, const std::string &variable, const Serializable *const value)
{
    return WriteVariable( stream, variable, reinterpret_cast<int>(value) );
}

// Helper function to write an object header in the form: begin = <object>;
const bool Serializable::WriteHeader(std::ostream &stream, const std::string &objectName)
{
    return WriteVariable( stream, "begin", objectName );
}

// Helper function to write an object footer in the form: end = <object>;
const bool Serializable::WriteFooter(std::ostream &stream, const std::string &objectName)
{
    return WriteVariable( stream, "end", objectName );
}

const bool Serializable::Read(std::istream &stream)
{
    DESERIALIZE_OBJECT( object, stream, Serializable )
    {
        int oldAddress;
        if( !Deserializer::ReadVariable( stream, "address", oldAddress ) )
            break;

        if( !Deserializer::Register( oldAddress, this ) )
            break;
    }

    return object.ReadResult();
}

const bool Serializable::Write(std::ostream &stream) const
{
    SERIALIZE_OBJECT( object, stream, Serializable )
    {
        if( !WriteVariable( stream, "address", this ) )
            break;
    }

    return object.WriteResult();
}

const bool Serializable::RestorePointers()
{
    // Note: Empty default implementation; override to fix any required pointers
    return true;
}
