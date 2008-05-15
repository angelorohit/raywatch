
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

#ifndef DESERIALIZERHELPER_HEADER
#define DESERIALIZERHELPER_HEADER

#include "Deserializer.h"

// Helper Macros

#define DESERIALIZE_CLASS(Identifier,Stream,ClassName)      \
    ObjectDeserializer Identifier( Stream, #ClassName );    \
    for( sizeof(ClassName); Identifier; ++Identifier )

//#define DESERIALIZE_OBJECT(Identifier,Stream,ObjectName)    \
//    ObjectDeserializer Identifier( Stream, ObjectName );    \
//    for( ; Identifier; ++Identifier )

#define DESERIALIZE_LIST(Identifier,Stream,ListName)        \
    ListDeserializer Identifier( Stream, ListName );        \
    for( ; Identifier; ++Identifier )


// Helper class to deserialize a container object
class ObjectDeserializer
{
private:
    const std::string    _name;     // Name of the object
    std::istream        &_stream;   // Reference to the stream from which this object is being read

    bool _bError;
    bool _bFinished;

public:
// Constructor
    explicit ObjectDeserializer(std::istream &stream, const std::string &name) :
        _name( name ),
        _stream( stream ),
        _bError( false ),
        _bFinished( false )
    {
        if( !Deserializer::ReadHeader( _stream, _name ) )
            _bError = true;
    }

private:
// Copy Constructor / Assignment Operator
    ObjectDeserializer(const ObjectDeserializer &);
    const ObjectDeserializer &operator =(const ObjectDeserializer &);

// Functions
public:
    // Should we continue with this object?
    operator const bool () const
    {
        return !(_bError || _bFinished);
    }

    // See if we've reached the end of the object or not
    void operator ++ ()
    {
        if( Deserializer::ReadFooter( _stream, _name ) )
            _bFinished = true;
        else
            _bError = true;
    }

    // The result of the read; true if we succeeded, false otherwise
    const bool ReadResult() const
    {
        return (!_bError) && _bFinished;
    }

    // Did we fail to read this object?
    const bool ReadFailed() const
    {
        return !ReadResult();
    }
};

// Helper class to deserialize a list of objects
class ListDeserializer
{
private:
    const std::string    _name;     // Name of the list of objects
    std::istream        &_stream;   // Reference to the stream from which this list is being read

    bool _bError;
    bool _bFinished;

public:
// Constructor
    explicit ListDeserializer(std::istream &stream, const std::string &name) :
        _name( name ),
        _stream( stream ),
        _bError( false ),
        _bFinished( false )
    {
        if( !Deserializer::ReadHeader( _stream, _name ) )
            _bError = true;
    }

private:
// Copy Constructor / Assignment Operator
    ListDeserializer(const ListDeserializer &);
    const ListDeserializer &operator =(const ListDeserializer &);

// Functions
public:
    // Should we continue with this list?
    operator const bool () const
    {
        return !(_bError || _bFinished);
    }

    // See if we've reached the end of the list or not
    void operator ++ ()
    {
        if( Deserializer::PeekFooter( _stream, _name ) )
            _bFinished = true;
    }

    // The result of the read; true if we succeeded, false otherwise
    const bool ReadResult() const
    {
        return (!_bError) && _bFinished;
    }

    // Did we fail to read this list?
    const bool ReadFailed() const
    {
        return !ReadResult();
    }
};

#endif
