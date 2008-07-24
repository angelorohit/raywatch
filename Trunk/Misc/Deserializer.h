
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

#ifndef DESERIALIZER_HEADER
#define DESERIALIZER_HEADER

#include "TokenStream.h"
#include "Vector.h"
#include <map>
#include <iostream>

// Forward Declarations
class Serializable;

class Deserializer
{
// Members
private:
    typedef std::map<std::size_t, Serializable*> AssocMap;

    AssocMap    _associations;
    TokenStream _stream;

public:
// Constructor
    explicit Deserializer();
// Destructor
    ~Deserializer();

private:
// Copy Constructor / Assignment Operator
    Deserializer(const Deserializer &);
    const Deserializer &operator =(const Deserializer &);

// Functions
private:
    // Restores the pointers of all Serializables currently registered in the associations map
    const bool RestoreAllPointers();

    // Reads a token and verifies it
    const bool ReadToken(const std::string &name, const std::string &delimiterSet);

    // A base ReadObject function; all other ReadObject functions use this function.
    const bool ReadObjectBase(const std::string &name, std::string &value);

public:
    const bool Open(std::istream &stream);
    void Close();

    // Helper functions to read group objects
    const bool PeekGroupObjectHeader(std::string &name);
    const bool ReadGroupObjectHeader(const std::string &name);
    const bool PeekGroupObjectFooter();
    const bool ReadGroupObjectFooter();

    // Helper functions to read various data types
    const bool ReadObject(const std::string &name, std::string &value);
    const bool ReadObject(const std::string &name, std::size_t &value);
    const bool ReadObject(const std::string &name, int &value);
    const bool ReadObject(const std::string &name, float &value);
    const bool ReadObject(const std::string &name, bool &value);
    const bool ReadObject(const std::string &name, Vector<int> &value);
    const bool ReadObject(const std::string &name, Vector<float> &value);

    // Templated function to read any pointer type
    template <typename T>
    const bool ReadObject(const std::string &name, T* &pPointer)
    {
        // Read the value
        std::size_t valueRead;
        if( !ReadObject( name, valueRead ) )
            return false;

        pPointer = reinterpret_cast<T*>(valueRead);
        return true;
    }

    // Registers a Serializable and it's old address for restoration of pointers later.
    const bool Register(const std::size_t oldAddress, Serializable *const pSerializable);

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
            std::cout << "Error: Failed to restore pointer; the object at the given old address (" << oldAddress << ") is not of the required type" << std::endl;
            return false;
        }

        return true;
    }

    // Reads and returns a Serializable
    Serializable *const Read();
};

#endif
