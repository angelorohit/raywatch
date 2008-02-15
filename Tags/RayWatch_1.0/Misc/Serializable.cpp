
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

#ifdef _WIN32
    #pragma warning( disable : 4311 )
#endif

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

const bool Serializable::ReadVariable(std::istream &stream, const std::string &variable, std::string &value) const
{
    return Deserializer::ParseVariable( stream, variable, value );
}

const bool Serializable::ReadVariable(std::istream &stream, const std::string &variable, int &value) const
{
    // Read the value
    std::string valueRead;
    if( !ReadVariable( stream, variable, valueRead ) )
        return false;

    // Convert the value from string to the required type
    if( !Utility::String::FromString( value, valueRead ) )
    {
        std::cout << "Error: '" << valueRead << "' is not a valid int." << std::endl;
        return false;
    }

    return true;
}

const bool Serializable::ReadVariable(std::istream &stream, const std::string &variable, float &value) const
{
    // Read the value
    std::string valueRead;
    if( !ReadVariable( stream, variable, valueRead ) )
        return false;

    // Convert the value from string to the required type
    if( !Utility::String::FromString( value, valueRead ) )
    {
        std::cout << "Error: '" << valueRead << "' is not a valid float." << std::endl;
        return false;
    }

    return true;
}

const bool Serializable::ReadVariable(std::istream &stream, const std::string &variable, bool &value) const
{
    // Read the value
    std::string valueRead;
    if( !ReadVariable( stream, variable, valueRead ) )
        return false;

    if( valueRead.compare( "true" ) == 0 )
    {
        value = true;
        return true;
    }

    if( valueRead.compare( "false" ) == 0 )
    {
        value = false;
        return true;
    }

    std::cout << "Error: '" << valueRead << "' is not a valid bool." << std::endl;
    return false;
}

const bool Serializable::ReadVariable(std::istream &stream, const std::string &variable, Vector<float> &value) const
{
    // Read the value
    std::string valueRead;
    if( !ReadVariable( stream, variable, valueRead ) )
        return false;

    std::istringstream iss(valueRead);
    std::string separator;
    if( (iss >> std::dec >> value.x >> separator >> value.y >> separator >> value.z).fail() )
    {
        std::cout << "Error: '" << valueRead << "' is not a valid Vector<float>." << std::endl;
        return false;
    }

    return true;
}

// Reads an object header of the form: begin = <objectName>;
const bool Serializable::ReadHeader(std::istream &stream, const std::string &objectName) const
{
    std::string readObjectName;
    if( !ReadVariable( stream, "begin", readObjectName ) )
        return false;

    if( objectName.compare( readObjectName ) != 0 )
    {
        std::cout << "Error: Begin of object '" << objectName << "' was expected, but '" << readObjectName << "' was found instead." << std::endl;
        return false;
    }

    return true;
}

// Reads an object footer of the form: end = <objectName>;
const bool Serializable::ReadFooter(std::istream &stream, const std::string &objectName) const
{
    std::string readObjectName;
    if( !ReadVariable( stream, "end", readObjectName ) )
        return false;

    if( objectName.compare( readObjectName ) != 0 )
    {
        std::cout << "Error: End of object '" << objectName << "' was expected, but '" << readObjectName << "' was found instead." << std::endl;
        return false;
    }

    return true;
}

const bool Serializable::WriteVariable(std::ostream &stream, const std::string &variable, const std::string &value) const
{
    return (stream << std::string( _indentation, ' ' ) << variable << " = " << value << ";" << std::endl).good();
}

const bool Serializable::WriteVariable(std::ostream &stream, const std::string &variable, const char *const value) const
{
    return WriteVariable( stream, variable, std::string( value ) );
}

const bool Serializable::WriteVariable(std::ostream &stream, const std::string &variable, const int &value) const
{
    return WriteVariable( stream, variable, Utility::String::ToString( value ) );
}

const bool Serializable::WriteVariable(std::ostream &stream, const std::string &variable, const float &value) const
{
    return WriteVariable( stream, variable, Utility::String::ToString( value ) );
}

const bool Serializable::WriteVariable(std::ostream &stream, const std::string &variable, const bool &value) const
{
    const std::string strValue = value? "true": "false";
    return WriteVariable( stream, variable, strValue );
}

const bool Serializable::WriteVariable(std::ostream &stream, const std::string &variable, const Vector<float> &value) const
{
    const std::string strValue =
        Utility::String::ToString(value.x) + ", " + Utility::String::ToString(value.y) + ", " + Utility::String::ToString(value.z);
    return WriteVariable( stream, variable, strValue );
}

const bool Serializable::WriteVariable(std::ostream &stream, const std::string &variable, const Serializable *const value) const
{
    return WriteVariable( stream, variable, (int)value );
}

// Helper function to write an object header in the form: begin = <object>;
const bool Serializable::WriteHeader(std::ostream &stream, const std::string &objectName) const
{
    return WriteVariable( stream, "begin", objectName );
}

// Helper function to write an object footer in the form: end = <object>;
const bool Serializable::WriteFooter(std::ostream &stream, const std::string &objectName) const
{
    return WriteVariable( stream, "end", objectName );
}

const bool Serializable::Read(std::istream &stream)
{
    int oldAddress;
    if( !ReadVariable( stream, "address", oldAddress ) )
        return false;

    if( !Deserializer::Register( oldAddress, this ) )
        return false;

    if( !ReadFooter( stream, "Serializable" ) )
        return false;

    return true;
}

const bool Serializable::Write(std::ostream &stream) const
{
    if( !WriteHeader( stream, "Serializable" ) )
        return false;

    Indent();
    {
        if( !WriteVariable( stream, "address", this ) )
            return false;
    }
    Unindent();

    if( !WriteFooter( stream, "Serializable" ) )
        return false;

    return true;
}

const bool Serializable::RestorePointers()
{
    // Note: Empty default implementation; override to fix any required pointers
    return true;
}
