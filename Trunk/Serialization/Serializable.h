
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

// Forward Declarations
class Deserializer;
class Serializer;
class AddressTranslator;

class Serializable
{
// Members
protected:
    mutable int _serializationDepth;

protected:
// Constructor
    explicit Serializable();
public:
// Destructor
    virtual ~Serializable();

// Functions
public:
    virtual const bool Read(Deserializer &d, void *const pUserData) =0;
    virtual const bool Write(Serializer &s) const =0;

    // This will be called by the Deserializer to give this Serializable object a chance to translate
    // all its old pointer addresses to the new ones via the TranslateAddress() function.
    // Default empty implementation is provided; override and implement if pointer translation is required.
    // Note: When overridden, the immediate base class' version must be called.
    virtual const bool RestorePointers(AddressTranslator &t);
};

#endif
