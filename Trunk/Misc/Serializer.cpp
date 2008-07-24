
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

#include "Serializer.h"
#include "Utility.h"

// Constructor
Serializer::Serializer(std::ostream &stream) :
    _indentation( 0 ),
    _stream( stream )
{
}

// Destructor
Serializer::~Serializer()
{
}

// Functions

void Serializer::Indent()
{
    _indentation += 4;
}

void Serializer::Unindent()
{
    _indentation -= 4;
}

// Helper functions to write group objects
const bool Serializer::WriteGroupObjectHeader(const std::string &name)
{
    return (_stream <<
        std::string(_indentation, ' ') << name << std::endl <<
        std::string(_indentation, ' ') << "{"  << std::endl ).good();
}

const bool Serializer::WriteGroupObjectFooter()
{
    return (_stream <<
        std::string(_indentation, ' ') << "}"  << std::endl ).good();
}

// A base WriteObject function; all other WriteObject functions use this function.
const bool Serializer::WriteObjectBase(const std::string &name, const std::string &value)
{
    return (_stream << std::string(_indentation, ' ') << name << " = " << value << ";" << std::endl).good();
}

// Helper functions to write various data types
const bool Serializer::WriteObject(const std::string &name, const std::string &value)
{
    const std::string doubleQuote( "\"" );
    return WriteObjectBase( name, doubleQuote + value + doubleQuote );
}

const bool Serializer::WriteObject(const std::string &name, const char *const value)
{
    return WriteObjectBase( name, std::string( value ) );
}

const bool Serializer::WriteObject(const std::string &name, const std::size_t &value)
{
    return WriteObjectBase( name, Utility::String::ToString( value ) );
}

const bool Serializer::WriteObject(const std::string &name, const int &value)
{
    return WriteObjectBase( name, Utility::String::ToString( value ) );
}

const bool Serializer::WriteObject(const std::string &name, const float &value)
{
    return WriteObjectBase( name, Utility::String::ToString( value ) );
}

const bool Serializer::WriteObject(const std::string &name, const bool &value)
{
    const std::string strValue( value? ("true"): ("false") );
    return WriteObjectBase( name, strValue );
}

const bool Serializer::WriteObject(const std::string &name, const Vector<int> &value)
{
    const std::string strValue =
        Utility::String::ToString(value.x) + ", " +
        Utility::String::ToString(value.y) + ", " +
        Utility::String::ToString(value.z);

    return WriteObjectBase( name, strValue );
}

const bool Serializer::WriteObject(const std::string &name, const Vector<float> &value)
{
    const std::string strValue =
        Utility::String::ToString(value.x) + ", " +
        Utility::String::ToString(value.y) + ", " +
        Utility::String::ToString(value.z);

    return WriteObjectBase( name, strValue );
}

const bool Serializer::WriteObject(const std::string &name, const Serializable *const value)
{
    return WriteObject( name, reinterpret_cast<std::size_t>(value) );
}
