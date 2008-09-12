
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
#include "Serializable.h"
#include "Utility.h"

// Constructor
Serializer::Serializer(std::ostream &stream) :
    _indentation( 0 ),
    _bSuppressNextWriteIndent( false ),
    _stream( stream )
{
}

// Destructor
Serializer::~Serializer()
{
}

// Functions

// Helper functions to write string literals to the stream (useful for writing custom data).
const bool Serializer::WriteIndentation()
{
    if( _bSuppressNextWriteIndent )
    {
        _bSuppressNextWriteIndent = false;
        return true;
    }

    return (_stream << std::string(_indentation, ' ')).good();
}

const bool Serializer::WriteString(const std::string &str)
{
    return (_stream << str).good();
}

const bool Serializer::WriteLine(const std::string &str)
{
    return (_stream << str << std::endl).good();
}

void Serializer::SuppressNextWriteIndent()
{
    _bSuppressNextWriteIndent = true;
}

// A base WriteObject function; all other WriteObject functions use this function.
const bool Serializer::WriteObjectBase(const std::string &name, const std::string &value)
{
    return WriteIndentation() && WriteString(name) && WriteString(" = ") && WriteString(value) && WriteLine(";");
}

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
    return  WriteIndentation() && WriteLine(name) &&
            WriteIndentation() && WriteLine("{");
}

const bool Serializer::WriteGroupObjectFooter()
{
    return  WriteIndentation() && WriteLine("}");
}

// Helper functions to write various data types
const bool Serializer::WriteObject(const std::string &name, const std::string &value, const DefaultValue<std::string> &defaultValue)
{
    if( defaultValue.Exists() && (value.compare( defaultValue.Get() ) == 0) )
        return true;

    const std::string doubleQuote( "\"" );
    return WriteObjectBase( name, doubleQuote + value + doubleQuote );
}

const bool Serializer::WriteObject(const std::string &name, const char *const value, const DefaultValue<const char *> &defaultValue)
{
    return WriteObject( name, std::string(value), std::string(defaultValue.Get()) );
}

const bool Serializer::WriteObject(const std::string &name, const std::size_t &value, const DefaultValue<std::size_t> &defaultValue)
{
    if( defaultValue.Exists() && (value == defaultValue.Get()) )
        return true;

    return WriteObjectBase( name, Utility::String::ToString( value ) );
}

const bool Serializer::WriteObject(const std::string &name, const int &value, const DefaultValue<int> &defaultValue)
{
    if( defaultValue.Exists() && (value == defaultValue.Get()) )
        return true;

    return WriteObjectBase( name, Utility::String::ToString( value ) );
}

const bool Serializer::WriteObject(const std::string &name, const float &value, const DefaultValue<float> &defaultValue)
{
    if( defaultValue.Exists() && (value == defaultValue.Get()) )
        return true;

    return WriteObjectBase( name, Utility::String::ToString( value ) );
}

const bool Serializer::WriteObject(const std::string &name, const bool &value, const DefaultValue<bool> &defaultValue)
{
    if( defaultValue.Exists() && (value == defaultValue.Get()) )
        return true;

    const std::string strValue( value? ("true"): ("false") );
    return WriteObjectBase( name, strValue );
}

const bool Serializer::WriteObject(const std::string &name, const Vector<int> &value, const DefaultValue<Vector<int> > &defaultValue)
{
    if( defaultValue.Exists() && (value == defaultValue.Get()) )
        return true;

    const std::string strValue =
        Utility::String::ToString(value.x) + ", " +
        Utility::String::ToString(value.y) + ", " +
        Utility::String::ToString(value.z);

    return WriteObjectBase( name, strValue );
}

const bool Serializer::WriteObject(const std::string &name, const Vector<float> &value, const DefaultValue<Vector<float> > &defaultValue)
{
    if( defaultValue.Exists() && (value == defaultValue.Get()) )
        return true;

    const std::string strValue =
        Utility::String::ToString(value.x) + ", " +
        Utility::String::ToString(value.y) + ", " +
        Utility::String::ToString(value.z);

    return WriteObjectBase( name, strValue );
}

// Writes a Serializable object
const bool Serializer::WriteObject(const std::string &name, const Serializable &value )
{
    // Write the variable name
    if( !WriteIndentation() ||
        !WriteString(name)  ||
        !WriteString(" = ") )
        return false;

    // We want the header to be immediately after the name (without being indented)
    SuppressNextWriteIndent();

    // Write the value
    return value.Write( *this );
}

// Writes a pointer to a Serializable object
const bool Serializer::WriteObject(
    const std::string &name,
    const Serializable *const value,
    const DefaultValue<const Serializable *> &defaultValue )
{
    if( defaultValue.Exists() && (value == defaultValue.Get()) )
        return true;

    const std::size_t address = reinterpret_cast<std::size_t>( value );
    return WriteObject( name, Utility::String::ToString( address ) );
}
