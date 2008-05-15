
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

#ifdef _WIN32
    #pragma warning( disable : 4311 )
#endif

// Initialize static members
int Serializer::_indentation( 0 );

// Functions

void Serializer::Indent()
{
    _indentation += 4;
}

void Serializer::Unindent()
{
    _indentation -= 4;
}

const bool Serializer::WriteVariable(std::ostream &stream, const std::string &variable, const std::string &value)
{
    return (stream << std::string( _indentation, ' ' ) << variable << " = " << value << ";" << std::endl).good();
}

const bool Serializer::WriteVariable(std::ostream &stream, const std::string &variable, const char *const value)
{
    return WriteVariable( stream, variable, std::string( value ) );
}

const bool Serializer::WriteVariable(std::ostream &stream, const std::string &variable, const int &value)
{
    return WriteVariable( stream, variable, Utility::String::ToString( value ) );
}

const bool Serializer::WriteVariable(std::ostream &stream, const std::string &variable, const float &value)
{
    return WriteVariable( stream, variable, Utility::String::ToString( value ) );
}

const bool Serializer::WriteVariable(std::ostream &stream, const std::string &variable, const bool &value)
{
    const std::string strValue = value? "true": "false";
    return WriteVariable( stream, variable, strValue );
}

const bool Serializer::WriteVariable(std::ostream &stream, const std::string &variable, const Vector<int> &value)
{
    const std::string strValue =
        Utility::String::ToString(value.x) + ", " +
        Utility::String::ToString(value.y) + ", " +
        Utility::String::ToString(value.z);
    
    return WriteVariable( stream, variable, strValue );
}

const bool Serializer::WriteVariable(std::ostream &stream, const std::string &variable, const Vector<float> &value)
{
    const std::string strValue =
        Utility::String::ToString(value.x) + ", " +
        Utility::String::ToString(value.y) + ", " +
        Utility::String::ToString(value.z);
    
    return WriteVariable( stream, variable, strValue );
}

const bool Serializer::WriteVariable(std::ostream &stream, const std::string &variable, const Serializable *const value)
{
    return WriteVariable( stream, variable, reinterpret_cast<int>(value) );
}

// Helper function to write an object header in the form: begin = <object>;
const bool Serializer::WriteHeader(std::ostream &stream, const std::string &objectName)
{
    return WriteVariable( stream, "begin", objectName );
}

// Helper function to write an object footer in the form: end = <object>;
const bool Serializer::WriteFooter(std::ostream &stream, const std::string &objectName)
{
    return WriteVariable( stream, "end", objectName );
}
