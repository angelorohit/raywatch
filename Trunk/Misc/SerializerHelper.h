
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

#ifndef SERIALIZERHELPER_HEADER
#define SERIALIZERHELPER_HEADER

#include "Serializer.h"
#include "Sink.h"

// Helper Macros

#define SERIALIZE_CLASS(Identifier,Stream,ClassName)        \
    ObjectSerializer Identifier( Stream, #ClassName );      \
    for( Sink(sizeof(ClassName)); Identifier; ++Identifier )

#define SERIALIZE_OBJECT(Identifier,Stream,ObjectName)      \
    ObjectSerializer Identifier( Stream, ObjectName );      \
    for( ; Identifier; ++Identifier )


// Helper class to serialize a container object
class ObjectSerializer
{
private:
    const std::string    _name;     // Name of the object
    std::ostream        &_stream;   // Reference to the stream to which this object is being written

    bool _bError;
    bool _bFinished;

public:
// Constructor
    explicit ObjectSerializer(std::ostream &stream, const std::string &name) :
        _name( name ),
        _stream( stream ),
        _bError( false ),
        _bFinished( false )
    {
        if( !Serializer::WriteHeader( _stream, _name ) )
            _bError = true;

        Serializer::Indent();
    }

private:
// Copy Constructor / Assignment Operator
    ObjectSerializer(const ObjectSerializer &);
    const ObjectSerializer &operator =(const ObjectSerializer &);

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
        Serializer::Unindent();

        if( Serializer::WriteFooter( _stream, _name ) )
            _bFinished = true;
        else
            _bError = true;
    }

    // The result of the write; true if we succeeded, false otherwise
    const bool WriteResult() const
    {
        return (!_bError) && _bFinished;
    }

    // Did we fail to write this object?
    const bool WriteFailed() const
    {
        return !WriteResult();
    }
};

#endif
