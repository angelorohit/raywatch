
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
    static int _indentation;

// Monostate related
private:
    // Constructor
    explicit Serializer();
    // Destructor
    ~Serializer();

    // Copy Constructor / Assignment Operator
    Serializer(const Serializer &);
    const Serializer &operator =(const Serializer &);

// Functions
public:
    static void Indent();
    static void Unindent();

    // Helper functions to write various data types
    static const bool WriteVariable(std::ostream &stream, const std::string &variable, const std::string &value);
    static const bool WriteVariable(std::ostream &stream, const std::string &variable, const char *const value);
    static const bool WriteVariable(std::ostream &stream, const std::string &variable, const int &value);
    static const bool WriteVariable(std::ostream &stream, const std::string &variable, const float &value);
    static const bool WriteVariable(std::ostream &stream, const std::string &variable, const bool &value);
    static const bool WriteVariable(std::ostream &stream, const std::string &variable, const Vector<int> &value);
    static const bool WriteVariable(std::ostream &stream, const std::string &variable, const Vector<float> &value);
    static const bool WriteVariable(std::ostream &stream, const std::string &variable, const Serializable *const value);

    // Helper function to write an object header in the form: begin = <objectName>;
    static const bool WriteHeader(std::ostream &stream, const std::string &objectName);
    // Helper function to write an object footer in the form: end = <objectName>;
    static const bool WriteFooter(std::ostream &stream, const std::string &objectName);
};

#endif
