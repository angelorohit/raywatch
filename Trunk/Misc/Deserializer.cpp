
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
#include <string>

// Initialize static members
Deserializer::AssocMap Deserializer::_associations;

// Parses an unknown variable of the format: <variable> = <value>;
const bool Deserializer::ParseVariable(std::istream &stream, std::string &variable, std::string &value)
{
    // Get the variable
    for(;;)
    {
        getline(stream, variable, '=');
        Utility::String::TrimWhiteSpaces( variable );

        // If we've reached the end of file
        if( stream.eof() )
        {
            std::cout << "Error: A Variable was expected, but EndOfFile was reached." << std::endl;
            return false;
        }

        // If this is a comment
        if( variable.compare( "//" ) == 0 )
        {
            getline(stream, value, ';'); // Read upto the end of the comment
            continue;
        }

        // We got the variable we wanted
        break;
    }

    // Get the value
    getline(stream, value, ';');
    Utility::String::TrimWhiteSpaces( value );

    // If we've reached the end of file
    if( stream.eof() )
    {
        std::cout << "Error: Value for Variable '" << variable << "' was expected, but EndOfFile was reached." << std::endl;
        return false;
    }

    return true;
}

// Parses a known variable of the format: <variable> = <value>;
const bool Deserializer::ParseVariable(std::istream &stream, const std::string &variable, std::string &value)
{
    std::string variableRead;
    if( !ParseVariable( stream, variableRead, value ) )
        return false;

    // See if the variable read is what is expected
    if( variableRead.compare( variable ) != 0 )
    {
        std::cout << "Error: Variable '" << variable << "' was expected, but '" << variableRead << "' was found instead." << std::endl;
        return false;
    }

    return true;
}

const bool Deserializer::Register(const int &oldAddress, Serializable *const pSerializable)
{
    // Try to insert this address into the map
    if( _associations.insert( AssocMap::value_type( oldAddress, pSerializable ) ).second )
        return true;

    // We couldn't insert it
    std::cout << "Error: Failed to Register address [" << oldAddress << "] with Deserializer" << std::endl;
    return false;
}

Serializable *const Deserializer::Read(std::istream &stream)
{
    Serializable *pSerializable = 0;

    BEGIN_CODE_BLOCK
    {
        // Read the first Object's type
        std::string objectType;
        if( !ParseVariable( stream, "begin", objectType ) )
            break;

        // Create the object
        pSerializable = ObjectFactory<Serializable>::Create( objectType );
        if( !pSerializable )
        {
            std::cout << "Error: Unknown Object found: " << objectType << std::endl;
            EXIT_CODE_BLOCK;
        }

        // Load the object
        if( !pSerializable->Read( stream ) )
        {
            SAFE_DELETE_SCALAR( pSerializable );
            EXIT_CODE_BLOCK;
        }

        // Restore all pointers
        if( !RestoreAllPointers() )
        {
            SAFE_DELETE_SCALAR( pSerializable );
            EXIT_CODE_BLOCK;
        }
    }
    END_CODE_BLOCK;

    return pSerializable;
}

const bool Deserializer::RestoreAllPointers()
{
    bool bRetVal = true;

    // Go through all the Serializables and restore their pointers
    for(AssocMap::const_iterator itr = _associations.begin();
        itr != _associations.end();
        ++itr)
    {
        Serializable *const pSerializable = itr->second;
        if( !pSerializable->RestorePointers() )
        {
            bRetVal = false;
            break;
        }
    }

    // Clear the associations map; we're done with it.
    _associations.clear();

    return bRetVal;
}
