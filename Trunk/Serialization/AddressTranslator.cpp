
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

#include "AddressTranslator.h"
#include "Serializable.h"
#include "ForEach.h"

// Constructor
AddressTranslator::AddressTranslator() :
    _associations()
{
}

// Destructor
AddressTranslator::~AddressTranslator()
{
}

// Functions

// Registers a Serializable and it's old address for restoration of pointers later.
const bool AddressTranslator::Register(const std::size_t oldAddress, Serializable *const pSerializable)
{
    // Try to insert this address into the map
    if( _associations.insert( AssocMap::value_type( oldAddress, pSerializable ) ).second )
        return true;

    // We couldn't insert it
    std::cout << "Error: Failed to Register address [" << oldAddress << "] with the Deserializer; it might have already been registered earlier." << std::endl;
    return false;
}

void AddressTranslator::BeginTranslations()
{
    _associations.clear();
}

const bool AddressTranslator::EndTranslations()
{
    bool bRetVal = true;

    // Go through all the Serializables and restore their pointers
    FOR_EACH( itr, AssocMap, _associations )
    {
        Serializable *const pSerializable = itr->second;
        if( !pSerializable->RestorePointers( *this ) )
        {
            bRetVal = false;
            break;
        }
    }

    _associations.clear();

    return bRetVal;
}
