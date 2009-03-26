
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
    _stream()
{
    // Set the token stream as the line number provider for the log.
    Log.SetLineNumberProvider( &_stream );
}

// Destructor
Deserializer::~Deserializer()
{
    // Dump the contents of the message log to standard output
    FOR_EACH( itr, MessageLog::MessageList, Log.Messages() )
        std::cout << "[" << itr->first << "] " << itr->second << std::endl;

    Close();
}

// Reads a token of specified length and verifies it
const bool Deserializer::ReadKnownToken(const std::string &name)
{
    std::string readName;
    if( !_stream.ReadToken( readName, name.length() ) )
    {
        Log << "Error: '" << name << "' was expected." << endl;
        return false;
    }
    if( readName.compare( name ) != 0 )
    {
        Log << "Error: '" << name << "' was expected, but '" << readName << "' was found instead." << endl;
        return false;
    }

    return true;
}

const bool Deserializer::PeekKnownToken(const std::string &name)
{
    std::string readName;
    if( !_stream.PeekToken( readName, name.length() ) )
        return false;

    return (readName.compare( name ) == 0);
}

// Reads an unknown token
const bool Deserializer::ReadUnknownToken(std::string &name, const char delimiter)
{
    const std::string delimiterStr( 1, delimiter );

    // Read the name upto the delimiter or a white space.
    if( !_stream.ReadToken( name, delimiterStr + Utility::String::WhitespaceCharSet(), true, false ) )
        return false;

    // Read the delimiter
    return ReadKnownToken( delimiterStr );
}

const bool Deserializer::PeekUnknownToken(std::string &name, const char delimiter)
{
    const std::string delimiterStr( 1, delimiter );

    // Peek the name upto the delimiter or a white space.
    return _stream.PeekToken( name, delimiterStr + Utility::String::WhitespaceCharSet() );
}

// Reads a known token
const bool Deserializer::ReadKnownToken(const std::string &name, const char delimiter)
{
    // Read an unknown token
    std::string readName;
    if( !ReadUnknownToken( readName, delimiter ) )
    {
        if( readName.size() == 0 )
            Log << "Error: '" << name << "' was expected." << endl;

        return false;
    }

    // See if the read name is expected.
    if( readName.compare( name ) != 0 )
    {
        Log << "Error: '" << name << "' was expected, but '" << readName << "' was found instead." << endl;
        return false;
    }

    return true;
}

const bool Deserializer::PeekKnownToken(const std::string &name, const char delimiter)
{
    // Peek an unknown token
    std::string readName;
    if( !PeekUnknownToken( readName, delimiter ) )
        return false;

    // See if the read name is expected.
    return (readName.compare( name ) == 0);
}

// Helper functions to read a token and convert it to a value
const bool Deserializer::ReadValue(std::size_t &value, const char delimiter)
{
    // Read an unknown token
    std::string valueRead;
    if( !ReadUnknownToken( valueRead, delimiter ) )
    {
        if( valueRead.size() == 0 )
            Log << "Error: unsigned-int expected" << endl;

        return false;
    }

    // Convert the value from string to the required type
    if( !Utility::String::FromString( value, valueRead ) )
    {
        Log << "Error: '" << valueRead << "' is not a valid unsigned-int." << endl;
        return false;
    }

    return true;
}

const bool Deserializer::ReadValue(int &value, const char delimiter)
{
    // Read an unknown token
    std::string valueRead;
    if( !ReadUnknownToken( valueRead, delimiter ) )
    {
        if( valueRead.size() == 0 )
            Log << "Error: int expected" << endl;

        return false;
    }

    // Convert the value from string to the required type
    if( !Utility::String::FromString( value, valueRead ) )
    {
        Log << "Error: '" << valueRead << "' is not a valid int." << endl;
        return false;
    }

    return true;
}

const bool Deserializer::ReadValue(float &value, const char delimiter)
{
    // Read an unknown token
    std::string valueRead;
    if( !ReadUnknownToken( valueRead, delimiter ) )
    {
        if( valueRead.size() == 0 )
            Log << "Error: float expected" << endl;

        return false;
    }

    // Convert the value from string to the required type
    if( !Utility::String::FromString( value, valueRead ) )
    {
        Log << "Error: '" << valueRead << "' is not a valid float." << endl;
        return false;
    }

    return true;
}

const bool Deserializer::ReadValue(bool &value, const char delimiter)
{
    // Read an unknown token
    std::string valueRead;
    if( !ReadUnknownToken( valueRead, delimiter ) )
    {
        if( valueRead.size() == 0 )
            Log << "Error: bool expected" << endl;

        return false;
    }

    // Convert the value from string to the required type
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

    Log << "Error: '" << valueRead << "' is not a valid bool." << endl;
    return false;
}

const bool Deserializer::Open(std::istream &stream)
{
    return _stream.Open( stream );
}

void Deserializer::Close()
{
    _stream.Close();
}

// Helper functions to read group objects
const bool Deserializer::PeekGroupObjectHeader(std::string &name)
{
    return PeekUnknownToken( name, '{' );
}

const bool Deserializer::ReadGroupObjectHeader(const std::string &name)
{
    return ReadKnownToken( name, '{' );
}

const bool Deserializer::PeekGroupObjectFooter()
{
    return PeekKnownToken( "}" );
}

const bool Deserializer::ReadGroupObjectFooter()
{
    return ReadKnownToken( "}" );
}

// Helper functions to read various data types
const bool Deserializer::ReadObject(const std::string &name, std::string &value, const DefaultValue<std::string> &defaultValue)
{
    if( defaultValue.Exists() && !PeekKnownToken( name, '=' ) )
    {
        value = defaultValue.Get();
        return true;
    }

    // Read the name and verify it
    if( !ReadKnownToken( name, '=' ) )
        return false;

    // Read a doubleQuote
    if( !ReadKnownToken( "\"" ) )
        return false;

    // Read the value upto the next doubleQuote preserving whitespace
    // Note: value is allowed to be empty, an empty string might be intended;
    //       that's why we don't check the return value of ReadToken here.
    _stream.ReadToken( value, "\"", false, false );

    // Read a doubleQuote
    if( !ReadKnownToken( "\"" ) )
        return false;

    // Read a semicolon
    return ReadKnownToken( ";" );
}

const bool Deserializer::ReadObject(const std::string &name, std::size_t &value, const DefaultValue<std::size_t> &defaultValue)
{
    if( defaultValue.Exists() && !PeekKnownToken( name, '=' ) )
    {
        value = defaultValue.Get();
        return true;
    }

    // Read the name and verify it
    if( !ReadKnownToken( name, '=' ) )
        return false;

    return ReadValue( value, ';' );
}

const bool Deserializer::ReadObject(const std::string &name, int &value, const DefaultValue<int> &defaultValue)
{
    if( defaultValue.Exists() && !PeekKnownToken( name, '=' ) )
    {
        value = defaultValue.Get();
        return true;
    }

    // Read the name and verify it
    if( !ReadKnownToken( name, '=' ) )
        return false;

    return ReadValue( value, ';' );
}

const bool Deserializer::ReadObject(const std::string &name, float &value, const DefaultValue<float> &defaultValue)
{
    if( defaultValue.Exists() && !PeekKnownToken( name, '=' ) )
    {
        value = defaultValue.Get();
        return true;
    }

    // Read the name and verify it
    if( !ReadKnownToken( name, '=' ) )
        return false;

    return ReadValue( value, ';' );
}

const bool Deserializer::ReadObject(const std::string &name, bool &value, const DefaultValue<bool> &defaultValue)
{
    if( defaultValue.Exists() && !PeekKnownToken( name, '=' ) )
    {
        value = defaultValue.Get();
        return true;
    }

    // Read the name and verify it
    if( !ReadKnownToken( name, '=' ) )
        return false;

    return ReadValue( value, ';' );
}

const bool Deserializer::ReadObject(const std::string &name, Vector<int> &value, const DefaultValue<Vector<int> > &defaultValue)
{
    if( defaultValue.Exists() && !PeekKnownToken( name, '=' ) )
    {
        value = defaultValue.Get();
        return true;
    }

    // Read the name and verify it
    if( !ReadKnownToken( name, '=' ) )
        return false;

    return
        ReadValue( value.x, ',' ) &&
        ReadValue( value.y, ',' ) &&
        ReadValue( value.z, ';' );
}

const bool Deserializer::ReadObject(const std::string &name, Vector<float> &value, const DefaultValue<Vector<float> > &defaultValue)
{
    if( defaultValue.Exists() && !PeekKnownToken( name, '=' ) )
    {
        value = defaultValue.Get();
        return true;
    }

    // Read the name and verify it
    if( !ReadKnownToken( name, '=' ) )
        return false;

    return
        ReadValue( value.x, ',' ) &&
        ReadValue( value.y, ',' ) &&
        ReadValue( value.z, ';' );
}

// Reads a Serializable object
const bool Deserializer::ReadObject(const std::string &name, Serializable &value, void *const pUserData)
{
    // Read the name and verify it
    if( !ReadKnownToken( name, '=' ) )
        return false;

    if( !value.Read( *this, pUserData ) )
    {
        Log << "Error: Failed to read Serializable object: " << name << endl;
        return false;
    }

    return true;
}

// Reads a pointer to a Serializable object
const bool Deserializer::ReadObject(const std::string &name, Serializable* &pPointer)
{
    // Read a string object
    std::string valueRead;
    if( !ReadObject( name, valueRead ) )
        return false;

    Utility::String::TrimWhiteSpaces( valueRead );
    if( valueRead.size() == 0 )
    {
        Log << "Error: An address must be a non-empty string." << endl;
        return false;
    }

    // Convert the string into an address
    std::size_t address;
    if( !Utility::String::FromString( address, valueRead ) )
    {
        // The string didn't literally contain an address (unsigned-int); it might
        // be a "named address". So use the CRC of the string as the address.
        address = Utility::String::CalculateCrc( valueRead );
    }

    if( address == 0 )
    {
        Log << "Error: An address must be a non-zero, positive integer." << endl;
        return false;
    }

    pPointer = reinterpret_cast<Serializable*>( address );
    return true;
}

// Reads and returns a Serializable
Serializable *const Deserializer::Deserialize(void *const pUserData)
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
            Log << "Error: Unknown Object found: " << objectType << endl;
            EXIT_CODE_BLOCK;
        }

        // Begin address translation
        AddressTranslator::BeginTranslations();

        // Load the object
        if( !pSerializable->Read( *this, pUserData ) )
        {
            Log << "Error: Failed to read Serializable object: " << objectType << endl;
            SafeDeleteScalar( pSerializable );
            EXIT_CODE_BLOCK;
        }

        // End address translation
        if( !AddressTranslator::EndTranslations() )
        {
            SafeDeleteScalar( pSerializable );
            EXIT_CODE_BLOCK;
        }
    }
    END_CODE_BLOCK;

    return pSerializable;
}
