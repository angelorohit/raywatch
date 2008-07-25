
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

#ifndef ADDRESSTRANSLATOR_HEADER
#define ADDRESSTRANSLATOR_HEADER

#include <map>
#include <iostream>

// Forward Declarations
class Serializable;

class AddressTranslator
{
//Friends
    friend class Serializable;

// Members
private:
    typedef std::map<std::size_t, Serializable*> AssocMap;
    AssocMap _associations;

protected:
// Constructor
    explicit AddressTranslator();
// Destructor
    ~AddressTranslator();

private:
// Copy Constructor / Assignment Operator
    AddressTranslator(const AddressTranslator &);
    const AddressTranslator &operator =(const AddressTranslator &);

// Functions
private:
    // Note: Used by Serializable
    // Registers a Serializable and it's old address for restoration of pointers later.
    const bool Register(const std::size_t oldAddress, Serializable *const pSerializable);

protected:
    // Note: Used by Deserializer
    void BeginTranslations();
    const bool EndTranslations();

public:
    // Note: Used by everyone else
    // Templated function which can translate any pointer from the old address
    // (when last saved) to the new address (after loading and reconstruction).
    template <typename T>
    const bool TranslateAddress(T* &pPointer)
    {
        if( !pPointer )
            return true;

        const std::size_t oldAddress = reinterpret_cast<std::size_t>(pPointer);

        AssocMap::const_iterator itr = _associations.find( oldAddress );
        if( itr == _associations.end() )
        {
            std::cout << "Error: Failed to restore pointer; could not find Serializable with old address: " << oldAddress << std::endl;
            return false;
        }

        pPointer = dynamic_cast<T*>(itr->second);
        if( !pPointer )
        {
            std::cout << "Error: Failed to restore pointer; the object at the given old address [" << oldAddress << "] is not of the required type" << std::endl;
            return false;
        }

        return true;
    }
};

#endif
