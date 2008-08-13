
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

#include "AddressTranslator.h"
#include "TokenStream.h"
#include "Vector.h"
#include "DefaultValue.h"

class Deserializer : public AddressTranslator
{
// Members
private:
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
    // Reads a token and verifies it
    const bool ReadToken(const std::string &name, const std::string &delimiterSet);
    const bool PeekToken(const std::string &name, const std::string &delimiterSet);

    // Reads a token of specified length and verifies it
    const bool ReadToken(const std::string &name);
    const bool PeekToken(const std::string &name);

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
    const bool ReadObject(const std::string &name, std::string   &value, const DefaultValue<std::string>    &defaultValue = DefaultValue<std::string>()    );
    const bool ReadObject(const std::string &name, std::size_t   &value, const DefaultValue<std::size_t>    &defaultValue = DefaultValue<std::size_t>()    );
    const bool ReadObject(const std::string &name, int           &value, const DefaultValue<int>            &defaultValue = DefaultValue<int>()            );
    const bool ReadObject(const std::string &name, float         &value, const DefaultValue<float>          &defaultValue = DefaultValue<float>()          );
    const bool ReadObject(const std::string &name, bool          &value, const DefaultValue<bool>           &defaultValue = DefaultValue<bool>()           );
    const bool ReadObject(const std::string &name, Vector<int>   &value, const DefaultValue<Vector<int> >   &defaultValue = DefaultValue<Vector<int> >()   );
    const bool ReadObject(const std::string &name, Vector<float> &value, const DefaultValue<Vector<float> > &defaultValue = DefaultValue<Vector<float> >() );

    // Templated function to read any pointer type
    template <typename T>
    const bool ReadObject(const std::string &name, T* &pPointer, const DefaultValue<T*> &defaultValue = DefaultValue<T*>() )
    {
        if( defaultValue.Exists() && !PeekToken( name, "=" ) )
        {
            pPointer = defaultValue.Get();
            return true;
        }

        // Read the value
        std::size_t valueRead;
        if( !ReadObject( name, valueRead ) )
            return false;

        pPointer = reinterpret_cast<T*>(valueRead);
        return true;
    }

    // Reads and returns a Serializable
    Serializable *const Read();
};

#endif
