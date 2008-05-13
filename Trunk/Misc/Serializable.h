
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

#ifndef SERIALIZABLE_HEADER
#define SERIALIZABLE_HEADER

#include "Vector.h"
#include <iostream>
#include <string>

class Serializable
{
// Members
private:
    static int _indentation;

protected:
// Constructor
    explicit Serializable();
public:
// Destructor
    virtual ~Serializable();

// Functions
protected:
    static void Indent();
    static void Unindent();

    // Helper functions to write various data types
    const bool WriteVariable(std::ostream &stream, const std::string &variable, const std::string &value) const;
    const bool WriteVariable(std::ostream &stream, const std::string &variable, const char *const value) const;
    const bool WriteVariable(std::ostream &stream, const std::string &variable, const int &value) const;
    const bool WriteVariable(std::ostream &stream, const std::string &variable, const float &value) const;
    const bool WriteVariable(std::ostream &stream, const std::string &variable, const bool &value) const;
    const bool WriteVariable(std::ostream &stream, const std::string &variable, const Vector<int> &value) const;
    const bool WriteVariable(std::ostream &stream, const std::string &variable, const Vector<float> &value) const;
    const bool WriteVariable(std::ostream &stream, const std::string &variable, const Serializable *const value) const;

    // Helper function to write an object header in the form: begin = <objectName>;
    const bool WriteHeader(std::ostream &stream, const std::string &objectName) const;
    // Helper function to write an object footer in the form: end = <objectName>;
    const bool WriteFooter(std::ostream &stream, const std::string &objectName) const;

public:
    virtual const bool Read(std::istream &stream) =0;
    virtual const bool Write(std::ostream &stream) const =0;

    // Function which will be called to give this Serializable object a chance to translate
    // all its old pointer addresses to the new ones via the TranslateAddress() function.
    // This function will be called just after this Serializable is Read().
    // Default empty implementation is provided; override and implement if pointer translation
    // is required.
    // Do not call the Base's RestorePointers() in any overridden function; it
    // will automatically be called separately.
    virtual const bool RestorePointers();
};

#endif
