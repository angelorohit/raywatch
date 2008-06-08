
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

// Initialize static members
Deserializer::AssocMap Deserializer::_associations;

// A class to encapsulate the cached read functionality
class CachedRead
{
private:
    std::string     _variable;
    std::string     _value;
    bool            _bAvailable;

public:
    // Accessors
    const std::string   &Variable()             { return _variable;     }
    const std::string   &Value()                { return _value;        }
    const bool          &IsAvailable() const    { return _bAvailable;   }

    // Constructor
    explicit CachedRead() :
        _variable(),
        _value(),
        _bAvailable( false )
    {
    }

    // Functions

    void Set(const std::string &variable, const std::string &value)
    {
        _variable   = variable;
        _value      = value;
        _bAvailable = true;
    }

    void Clear()
    {
        _variable   .clear();
        _value      .clear();
        _bAvailable = false;
    }
};

// Parses an unknown variable of the format: <variable> = <value>;
const bool Deserializer::ParseVariable(
    std::istream    &stream,
    std::string     &variable,
    std::string     &value,
    const bool      &bPeek )
{
    static CachedRead cachedRead;

    // If the previous read was a peek, then return the cached variable/value now
    if( cachedRead.IsAvailable() )
    {
        variable = cachedRead.Variable();
        value    = cachedRead.Value();

        // If this read is also a peek, then the cached variable/value
        // should be left intact for the next read.
        if( !bPeek )
            cachedRead.Clear();

        return true;
    }

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

    // If this is a peek, then the same variable/value must be returned the next time.
    if( bPeek )
        cachedRead.Set( variable, value );

    return true;
}

const bool Deserializer::ReadVariable(std::istream &stream, const std::string &variable, std::string &value)
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

const bool Deserializer::ReadVariable(std::istream &stream, const std::string &variable, int &value)
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

const bool Deserializer::ReadVariable(std::istream &stream, const std::string &variable, float &value)
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

const bool Deserializer::ReadVariable(std::istream &stream, const std::string &variable, bool &value)
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

const bool Deserializer::ReadVariable(std::istream &stream, const std::string &variable, Vector<int> &value)
{
    // Read the value
    std::string valueRead;
    if( !ReadVariable( stream, variable, valueRead ) )
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

const bool Deserializer::ReadVariable(std::istream &stream, const std::string &variable, Vector<float> &value)
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
const bool Deserializer::ReadHeader(std::istream &stream, const std::string &objectName)
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
const bool Deserializer::ReadFooter(std::istream &stream, const std::string &objectName)
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

// Peeks whether we've reached an object header, but doesn't remove anything.
const bool Deserializer::PeekHeader(std::istream &stream, std::string &objectName)
{
    std::string variable;
    if( !ParseVariable( stream, variable, objectName, true ) )
        return false;

    return (variable.compare( "begin" ) == 0);
}

// Peeks whether we've reached an object footer, and removes it if we have.
const bool Deserializer::PeekFooter(std::istream &stream, const std::string &objectName)
{
    std::string variable, value;
    if( !ParseVariable( stream, variable, value, true ) )
        return false;

    if( (variable.compare( "end" ) != 0) || (value.compare( objectName ) != 0) )
        return false;

    // Now that we're sure it's the required footer, remove it by reading it.
    if( !ReadFooter( stream, objectName ) )
        return false;

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
        // Peek the first object's header
        std::string objectType;
        if( !PeekHeader( stream, objectType ) )
            break;

        // Create the object
        pSerializable = ObjectFactory<Serializable>::Instance().Create( objectType );
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
    FOR_EACH( itr, AssocMap, _associations )
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
