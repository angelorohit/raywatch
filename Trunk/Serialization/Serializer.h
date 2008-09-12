
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

#ifndef SERIALIZER_HEADER
#define SERIALIZER_HEADER

#include "Vector.h"
#include "DefaultValue.h"
#include <iostream>
#include <string>

// Forward Declarations
class Serializable;

class Serializer
{
// Members
private:
    int              _indentation;
    bool             _bSuppressNextWriteIndent;
    std::ostream    &_stream;

public:
// Constructor
    explicit Serializer(std::ostream &stream);
// Destructor
    ~Serializer();

private:
// Copy Constructor / Assignment Operator
    Serializer(const Serializer &);
    const Serializer &operator =(const Serializer &);

// Functions
private:
    // Helper functions to write string literals to the stream (useful for writing custom data).
    const bool WriteIndentation();
    const bool WriteString(const std::string &str);
    const bool WriteLine(const std::string &str);
    void SuppressNextWriteIndent();

    // A base WriteObject function; all other WriteObject functions use this function.
    const bool WriteObjectBase(const std::string &name, const std::string &value);

public:
    void Indent();
    void Unindent();

    // Helper functions to write group objects
    const bool WriteGroupObjectHeader(const std::string &name);
    const bool WriteGroupObjectFooter();

    // Helper functions to write various data types
    const bool WriteObject(const std::string &name, const std::string   &value, const DefaultValue<std::string>    &defaultValue = DefaultValue<std::string>()    );
    const bool WriteObject(const std::string &name, const char *const    value, const DefaultValue<const char *>   &defaultValue = DefaultValue<const char *>()   );
    const bool WriteObject(const std::string &name, const std::size_t   &value, const DefaultValue<std::size_t>    &defaultValue = DefaultValue<std::size_t>()    );
    const bool WriteObject(const std::string &name, const int           &value, const DefaultValue<int>            &defaultValue = DefaultValue<int>()            );
    const bool WriteObject(const std::string &name, const float         &value, const DefaultValue<float>          &defaultValue = DefaultValue<float>()          );
    const bool WriteObject(const std::string &name, const bool          &value, const DefaultValue<bool>           &defaultValue = DefaultValue<bool>()           );
    const bool WriteObject(const std::string &name, const Vector<int>   &value, const DefaultValue<Vector<int> >   &defaultValue = DefaultValue<Vector<int> >()   );
    const bool WriteObject(const std::string &name, const Vector<float> &value, const DefaultValue<Vector<float> > &defaultValue = DefaultValue<Vector<float> >() );

    // Writes a Serializable object
    const bool WriteObject(const std::string &name, const Serializable &value );

    // Writes a pointer to a Serializable object
    const bool WriteObject(
        const std::string &name,
        const Serializable *const value,
        const DefaultValue<const Serializable *> &defaultValue = DefaultValue<const Serializable *>() );
};

#endif
