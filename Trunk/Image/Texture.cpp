
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

#include "Texture.h"
#include "ObjectFactory.h"
#include "Deserializer.h"
#include "DeserializerHelper.h"
#include "SerializerHelper.h"
#include "Utility.h"

// Register with the ObjectFactory
ObjectFactory_Register(Serializable, Texture);

// Constructor
Texture::Texture() :
    _image(),
    _fileName()
{
}

// Destructor
Texture::~Texture()
{
    _image.Release();
    _fileName.clear();
}

// Functions

// Accessors
const std::string &Texture::FileName() const
{
    return _fileName;
}

const bool Texture::Load(const std::string &fileName)
{
    // Release the previous one
    _image.Release();
    _fileName.clear();

    if( !_image.SDL_Load( fileName ) )
    {
        std::cout << "Error: Failed to load image from file: " << fileName << std::endl;
        return false;
    }

    _fileName = fileName;

    return true;
}

const Pixel<float> Texture::GetPixel(const float &tu, const float &tv) const
{
    return _image.GetPixel( tu, tv );
}

// Serializable's functions

const bool Texture::Read(Deserializer &d)
{
    DESERIALIZE_CLASS( object, d, Texture )
    {
        // Read the base
        if( !Serializable::Read( d ) )
            break;

        // Read the texture fileName
        {
            std::string fileName;
            if( !d.ReadObject( "fileName", fileName ) )
                break;

            Utility::String::TrimWhiteSpaces( fileName );
            if( fileName.empty() )
            {
                std::cout << "Error: file name cannot be an empty string." << std::endl;
                break;
            }

            if( !Load( fileName ) )
                break;
        }
    }

    return object.ReadResult();
}

const bool Texture::Write(Serializer &s) const
{
    SERIALIZE_CLASS( object, s, Texture )
    {
        // Write the base
        if( !Serializable::Write( s ) )
            break;

        if( !s.WriteObject( "fileName", _fileName ) )
            break;
    }

    return object.WriteResult();
}
