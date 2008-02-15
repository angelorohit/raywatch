
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

#include <iostream>
#include <map>

// Forward Declarations
class Serializable;

class Deserializer
{
// Members
private:
    typedef std::map<int, Serializable*> AssocMap;
    static AssocMap _associations;

// Monostate related
private:
    // Constructor
    explicit Deserializer();
    // Destructor
    ~Deserializer();

    // Copy Constructor / Assignment Operator
    Deserializer(const Deserializer &);
    const Deserializer &operator =(const Deserializer &);

// Functions
private:
    // Restores the pointers of all Serializables currently registered in the associations map
    static const bool RestoreAllPointers();

public:
    // Parses an unknown variable of the format: <variable> = <value>;
    static const bool ParseVariable(std::istream &stream, std::string &variable, std::string &value);
    // Parses a known variable of the format: <variable> = <value>;
    static const bool ParseVariable(std::istream &stream, const std::string &variable, std::string &value);

    // Registers a Serializable and it's old address for restoration of pointers later.
    static const bool Register(const int &oldAddress, Serializable *const pSerializable);

    // Templated function which can translate any pointer from the old address
    // (when last saved) to the new address (after loading and reconstruction).
    template <typename T>
    static const bool TranslateAddress(T* &pPointer)
    {
        if( !pPointer )
            return true;

        const int oldAddress = (int)pPointer;

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

    // Reads and returns a Serializable from a stream
    static Serializable *const Read(std::istream &stream);
};

#endif
