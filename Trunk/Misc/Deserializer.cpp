
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

#include "Deserializer.h"
#include "CodeBlocks.h"
#include "SafeDelete.h"
#include "Utility.h"
#include "ObjectFactory.h"
#include "Serializable.h"
#include "ForEach.h"
#include <string>

// Constructor
Deserializer::Deserializer() :
    _associations(),
    _stream()
{
}

// Destructor
Deserializer::~Deserializer()
{
    Close();
}

const bool Deserializer::Open(std::istream &stream)
{
    return _stream.Open( stream );
}

void Deserializer::Close()
{
    AssocMap().swap( _associations );
    _stream.Close();
}

// Reads a token and verifies it
const bool Deserializer::ReadToken(const std::string &name, const std::string &delimiterSet)
{
    std::string readName;
    if( !_stream.ReadToken( readName, delimiterSet ) )
    {
        std::cout << "Error: Object '" << name << "' was expected." << std::endl;
        return false;
    }
    if( readName.compare( name ) != 0 )
    {
        std::cout << "Error: Object '" << name << "' was expected, but '" << readName << "' was found instead." << std::endl;
        return false;
    }

    return true;
}

// Helper functions to read group objects
const bool Deserializer::PeekGroupObjectHeader(std::string &name)
{
    return _stream.PeekToken( name, "{" );
}

const bool Deserializer::ReadGroupObjectHeader(const std::string &name)
{
    return ReadToken( name, "{" );
}

const bool Deserializer::PeekGroupObjectFooter()
{
    return _stream.PeekToken( "}" );
}

const bool Deserializer::ReadGroupObjectFooter()
{
    if( !_stream.ReadToken( "}" ) )
    {
        std::cout << "Error: Missing '}'" << std::endl;
        return false;
    }

    return true;
}

// A base ReadObject function; all other ReadObject functions use this function.
const bool Deserializer::ReadObjectBase(const std::string &name, std::string &value)
{
    // Read the name and verify it
    if( !ReadToken( name, "=" ) )
        return false;

    // Read the value
    if( !_stream.ReadToken( value, ";" ) )
    {
        std::cout << "Error: Missing ';'" << std::endl;
        return false;
    }

    return true;
}

// Helper functions to read various data types
const bool Deserializer::ReadObject(const std::string &name, std::string &value)
{
    // Read the name and verify it
    if( !ReadToken( name, "=" ) )
        return false;

    // Read a doubleQuote
    if( !_stream.ReadToken( "\"" ) )
    {
        std::cout << "Error: A double-quote (\") was expected (string values must be enclosed within double-quotes)." << std::endl;
        return false;
    }

    // Read the value upto the next doubleQuote preserving whitespace
    if( !_stream.ReadToken( value, "\"", false ) )
    {
        std::cout << "Error: A double-quote (\") was expected (string values must be enclosed within double-quotes)." << std::endl;
        return false;
    }

    // Read a semicolon
    if( !_stream.ReadToken( ";" ) )
    {
        std::cout << "Error: Missing ';'" << std::endl;
        return false;
    }

    return true;
}

const bool Deserializer::ReadObject(const std::string &name, std::size_t &value)
{
    std::string valueRead;
    if( !ReadObjectBase( name, valueRead ) )
        return false;

    // Convert the value from string to the required type
    if( !Utility::String::FromString( value, valueRead ) )
    {
        std::cout << "Error: '" << valueRead << "' is not a valid unsigned-int." << std::endl;
        return false;
    }

    return true;
}

const bool Deserializer::ReadObject(const std::string &name, int &value)
{
    std::string valueRead;
    if( !ReadObjectBase( name, valueRead ) )
        return false;

    // Convert the value from string to the required type
    if( !Utility::String::FromString( value, valueRead ) )
    {
        std::cout << "Error: '" << valueRead << "' is not a valid int." << std::endl;
        return false;
    }

    return true;
}

const bool Deserializer::ReadObject(const std::string &name, float &value)
{
    std::string valueRead;
    if( !ReadObjectBase( name, valueRead ) )
        return false;

    // Convert the value from string to the required type
    if( !Utility::String::FromString( value, valueRead ) )
    {
        std::cout << "Error: '" << valueRead << "' is not a valid float." << std::endl;
        return false;
    }

    return true;
}

const bool Deserializer::ReadObject(const std::string &name, bool &value)
{
    std::string valueRead;
    if( !ReadObjectBase( name, valueRead ) )
        return false;

    if( Utility::String::CaseInsensitiveCompare( valueRead, "true" ) == 0 )
    {
        value = true;
        return true;
    }
    if( Utility::String::CaseInsensitiveCompare( valueRead, "false" ) == 0 )
    {
        value = false;
        return true;
    }

    std::cout << "Error: '" << valueRead << "' is not a valid bool." << std::endl;
    return false;
}

const bool Deserializer::ReadObject(const std::string &name, Vector<int> &value)
{
    std::string valueRead;
    if( !ReadObjectBase( name, valueRead ) )
        return false;

    std::istringstream iss(valueRead);
    std::string separator;
    if( (iss >> std::dec >> value.x >> separator >> value.y >> separator >> value.z).fail() )
    {
        std::cout << "Error: '" << valueRead << "' is not a valid Vector<int>." << std::endl;
        return false;
    }

    return true;
}

const bool Deserializer::ReadObject(const std::string &name, Vector<float> &value)
{
    std::string valueRead;
    if( !ReadObjectBase( name, valueRead ) )
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

// Registers a Serializable and it's old address for restoration of pointers later.
const bool Deserializer::Register(const std::size_t oldAddress, Serializable *const pSerializable)
{
    // Try to insert this address into the map
    if( _associations.insert( AssocMap::value_type( oldAddress, pSerializable ) ).second )
        return true;

    // We couldn't insert it
    std::cout << "Error: Failed to Register address [" << oldAddress << "] with the Deserializer" << std::endl;
    return false;
}

// Reads and returns a Serializable
Serializable *const Deserializer::Read()
{
    Serializable *pSerializable = 0;

    BEGIN_CODE_BLOCK
    {
        // Peek the first object's header
        std::string objectType;
        if( !PeekGroupObjectHeader( objectType ) )
            break;

        // Create the object
        pSerializable = ObjectFactory<Serializable>::Instance().Create( objectType );
        if( !pSerializable )
        {
            std::cout << "Error: Unknown Object found: " << objectType << std::endl;
            EXIT_CODE_BLOCK;
        }

        // Load the object
        if( !pSerializable->Read( *this ) )
        {
            SafeDeleteScalar( pSerializable );
            EXIT_CODE_BLOCK;
        }

        // Restore all pointers
        if( !RestoreAllPointers() )
        {
            SafeDeleteScalar( pSerializable );
            EXIT_CODE_BLOCK;
        }
    }
    END_CODE_BLOCK;

    return pSerializable;
}

// Restores the pointers of all Serializables currently registered in the associations map
const bool Deserializer::RestoreAllPointers()
{
    bool bRetVal = true;

    // Go through all the Serializables and restore their pointers
    FOR_EACH( itr, AssocMap, _associations )
    {
        Serializable *const pSerializable = itr->second;
        if( !pSerializable->RestorePointers( *this ) )
        {
            bRetVal = false;
            break;
        }
    }

    // Clear the associations map; we're done with it.
    _associations.clear();

    return bRetVal;
}
