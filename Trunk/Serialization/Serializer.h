
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
#include <iostream>
#include <string>

// Forward Declarations
class Serializable;

class Serializer
{
// Members
private:
    int              _indentation;
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
    // A base WriteObject function; all other WriteObject functions use this function.
    const bool WriteObjectBase(const std::string &name, const std::string &value);

public:
    void Indent();
    void Unindent();

    // Helper functions to write group objects
    const bool WriteGroupObjectHeader(const std::string &name);
    const bool WriteGroupObjectFooter();

    // Helper functions to write various data types
    const bool WriteObject(const std::string &name, const std::string &value);
    const bool WriteObject(const std::string &name, const char *const value);
    const bool WriteObject(const std::string &name, const std::size_t &value);
    const bool WriteObject(const std::string &name, const int &value);
    const bool WriteObject(const std::string &name, const float &value);
    const bool WriteObject(const std::string &name, const bool &value);
    const bool WriteObject(const std::string &name, const Vector<int> &value);
    const bool WriteObject(const std::string &name, const Vector<float> &value);
    const bool WriteObject(const std::string &name, const Serializable *const value);
};

#endif
