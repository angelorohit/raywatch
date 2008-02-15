
//  RayWatch - A simple cross-platform RayTracer.
//  Copyright (C) 2008  Angelo Rohit Joseph Pulikotil
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

#ifndef IMAGE_HEADER
#define IMAGE_HEADER

#include "Pixel.h"
#include <string>
#include <stdio.h>

class Image
{
// Members
private:
    int              _width;
	int              _height;
	int              _numTotalPixels;
	Pixel<float>    *_pPixelData;

public:
    // Constructor
    explicit Image();
    // Destructor
    ~Image();

private:
    // Copy Constructor / Assignment Operator
	Image(const Image &);
	const Image &operator =(const Image &);

    // Functions
private:
    const bool SaveBMP(FILE *const outF) const;

public:
    // Load using SDL_Image's IMG_Load() function.
    const bool SDL_Load(const std::string &fileName);

    // Create a new empty image with the specified dimensions.
    const bool Create(const int &width, const int &height);

    const bool Save(const std::string &fileName) const;

    void Release();

	const int &Width() const;
	const int &Height() const;

	const bool GetPixel(const int &x, const int &y, Pixel<float> &pixel) const;
	const bool SetPixel(const int &x, const int &y, const Pixel<float> &pixel);

	const bool GetPixel(const int &x, const int &y, Pixel<> &pixel) const;
	const bool SetPixel(const int &x, const int &y, const Pixel<> &pixel);

    const Pixel<float> GetPixel(const float &tu, const float &tv) const;

	const Pixel<float> *const GetRawPixelData() const;
};

#endif
