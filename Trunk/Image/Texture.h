
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

#ifndef TEXTURE_HEADER
#define TEXTURE_HEADER

#include "Image.h"
#include "Serializable.h"
#include <string>

class Texture : public Serializable
{
// Members
private:
    Image           _image;
    std::string     _fileName;

public:
// Constructor
    explicit Texture();
// Destructor
    virtual ~Texture();

private:
// Copy Constructor / Assignment Operator
    Texture(const Texture &);
    const Texture &operator =(const Texture &);

// Functions
public:
    // Accessors
    const std::string &FileName() const;

    const bool Load(const std::string &fileName);
    const Pixel<float> GetPixel(const float &tu, const float &tv) const;

    // Serializable's functions
    virtual const bool Read(Deserializer &d, void *const pUserData);
    virtual const bool Write(Serializer &s) const;
};

#endif
