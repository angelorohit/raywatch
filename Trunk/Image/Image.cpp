
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

#include "Image.h"
#include "SafeDelete.h"
#include "CodeBlocks.h"
#include "Utility.h"
#include "Maths.h"
#include "Vector.h"
#include <SDL_Image.h>

// Constructor
Image::Image() :
    _width(0),
    _height(0),
    _numTotalPixels(0),
    _pPixelData(0)
{
}

// Destructor
Image::~Image()
{
    // Uninitialize if not already done
    Release();
}

// Functions

const bool Image::SDL_Load(const std::string &fileName)
{
    // Load the image using SDL_Image
    SDL_Surface *pSurface = IMG_Load( fileName.c_str() );
    if( !pSurface )
        return false;

    // Release any currently loaded data
    Release();

    // The return value
	bool bRetVal = false;

    BEGIN_CODE_BLOCK
    {
        // Set the width, height and total number of pixels
        _width  = pSurface->w;
        _height = pSurface->h;
        _numTotalPixels = _width * _height;

        // Allocate memory to hold the pixel data
        _pPixelData = new Pixel<float> [_numTotalPixels];
        if( !_pPixelData )
            EXIT_CODE_BLOCK;

        // Copy the pixel data from the SDL_Surface to our structure
        Pixel<> pixel;
        for(int y=0; y < _height; ++y)
        for(int x=0; x < _width; ++x)
        {
            // Get the color at this pixel location
            SDL_Color sdl_color;
            {
                Uint32 col = 0;
                //determine position
                const char *pPosition = (const char *)pSurface->pixels;
                //offset by y
                pPosition += (pSurface->pitch * y);
                //offset by x
                pPosition += (pSurface->format->BytesPerPixel * x);
                //copy pixel data
                memcpy( &col, pPosition, pSurface->format->BytesPerPixel );
                //convert color
                SDL_GetRGBA( col, pSurface->format, &sdl_color.r, &sdl_color.g, &sdl_color.b, &sdl_color.unused );
            }

            // Set our pixel with the color from the SDL_Surface pixel.
            pixel.Set(sdl_color.r, sdl_color.g, sdl_color.b, sdl_color.unused);
            SetPixel(x, y, pixel);
        }

        // Everything passed
        bRetVal = true;
    }
    END_CODE_BLOCK;

    // Free the surface
    SDL_FreeSurface( pSurface );

    // If the load was not successful, then
    // clean up anything partially loaded
    if( !bRetVal )
        Release();

    return bRetVal;
}

const bool Image::Create(const int &width, const int &height)
{
    // Release any currently loaded data
    Release();

    // The return value
	bool bRetVal = false;

    BEGIN_CODE_BLOCK
    {
        // Set the width, height and total number of pixels
        _width  = width;
        _height = height;
        _numTotalPixels = _width * _height;

        // Allocate memory to hold the pixel data
        _pPixelData = new Pixel<float> [_numTotalPixels];
        if( !_pPixelData )
            EXIT_CODE_BLOCK;

        // Everything passed
        bRetVal = true;
    }
    END_CODE_BLOCK;

    // If the load was not successful, then
    // clean up anything partially loaded
    if( !bRetVal )
        Release();

    return bRetVal;
}

const bool Image::Save(const std::string &fileName) const
{
    // Get the file name extension
    const std::string fileNameExt = fileName.substr( fileName.find_last_of( '.' ) + 1 );

    // Open the file
    FILE *const outF = fopen( fileName.c_str(), "wb" );
    if( outF == (FILE *)NULL )
        return false;

    // The return value
	bool bRetVal = false;

    // Process the file according to the extension
    BEGIN_CODE_BLOCK
    {
        // Bitmap
        if( Utility::String::CaseInsensitiveCompare( fileNameExt, "bmp" ) == 0 )
        {
            bRetVal = SaveBMP( outF );
            EXIT_CODE_BLOCK;
        }

        // Insert support for additional file formats just above this line.
    }
    END_CODE_BLOCK;

    // Close the file
	fclose( outF );

    return bRetVal;
}

void Image::Release()
{
    _width          = 0;
    _height         = 0;
    _numTotalPixels = 0;

    SAFE_DELETE_ARRAY( _pPixelData );
}

const int &Image::Width() const
{
    return _width;
}

const int &Image::Height() const
{
    return _height;
}

const bool Image::GetPixel(const int &x, const int &y, Pixel<float> &pixel) const
{
    if( _pPixelData             &&
        x >= 0 && x < _width    &&
        y >= 0 && y < _height   )
    {
        pixel = _pPixelData[ (y * _width + x) ];
        return true;
    }

    return false;
}

const bool Image::SetPixel(const int &x, const int &y, const Pixel<float> &pixel)
{
    if( _pPixelData             &&
        x >= 0 && x < _width    &&
        y >= 0 && y < _height   )
    {
        _pPixelData[ (y * _width + x) ] = pixel;
        return true;
    }

    return false;
}

const bool Image::GetPixel(const int &x, const int &y, Pixel<> &pixel) const
{
    if( _pPixelData             &&
        x >= 0 && x < _width    &&
        y >= 0 && y < _height   )
    {
        const Pixel<float> &retVal = _pPixelData[ (y * _width + x) ];

        pixel.Set(
            (unsigned char)Maths::Bound<float>(retVal._r * 255, 0, 255),
            (unsigned char)Maths::Bound<float>(retVal._g * 255, 0, 255),
            (unsigned char)Maths::Bound<float>(retVal._b * 255, 0, 255),
            (unsigned char)Maths::Bound<float>(retVal._a * 255, 0, 255) );

        return true;
    }

    return false;
}

const bool Image::SetPixel(const int &x, const int &y, const Pixel<> &pixel)
{
    if( _pPixelData             &&
        x >= 0 && x < _width    &&
        y >= 0 && y < _height   )
    {
        _pPixelData[ (y * _width + x) ].Set(
            pixel._r * (1.0f / 255.0f),
            pixel._g * (1.0f / 255.0f),
            pixel._b * (1.0f / 255.0f),
            pixel._a * (1.0f / 255.0f) );

        return true;
    }

    return false;
}

const Pixel<float> Image::GetPixel(const float &tu, const float &tv) const
{
    // Translate from [0..1][0..1] to [0.._width][0.._height]
    const float u = Maths::Abs( tu ) * _width;
    const float v = Maths::Abs( tv ) * _height;

    // Indexes of the texels
    const int u1 = (int)(u + 0) % _width;
    const int v1 = (int)(v + 0) % _height;
    const int u2 = (int)(u + 1) % _width;
    const int v2 = (int)(v + 1) % _height;

    // Fractional parts
    const float fu = u - (int)u;
    const float fv = v - (int)v;

    // Weights
    const float w1 = (1 - fu) * (1 - fv);
    const float w2 = fu * (1 - fv);
    const float w3 = fu * fv;
    const float w4 = (1 - fu) * fv;

    // Pixels
    const Pixel<float> &p1 = _pPixelData[ (u1 + v1 * _width) ];
    const Pixel<float> &p2 = _pPixelData[ (u2 + v1 * _width) ];
    const Pixel<float> &p3 = _pPixelData[ (u2 + v2 * _width) ];
    const Pixel<float> &p4 = _pPixelData[ (u1 + v2 * _width) ];

    // Weighted average
    return p1*w1 + p2*w2 + p3*w3 + p4*w4;
}

const Pixel<float> *const Image::GetRawPixelData() const
{
    return _pPixelData;
}

const bool Image::SaveBMP(FILE *const outF) const
{
    typedef unsigned char   BYTE;
	typedef unsigned short  WORD;
	typedef unsigned int    DWORD;
	typedef int             LONG;

	struct
	{
        WORD    bfType;
		DWORD   bfSize;
		WORD    bfReserved1;
		WORD    bfReserved2;
		DWORD   bfOffBits;

    } bmpFileHeader;

	struct
	{
        DWORD       biSize;
		LONG        biWidth;
		LONG        biHeight;
		WORD        biPlanes;
		WORD        biBitCount;
		DWORD       biCompression;
		DWORD       biSizeImage;
		LONG        biXPelsPerMeter;
		LONG        biYPelsPerMeter;
		DWORD       biClrUsed;
		DWORD       biClrImportant;

    } bmpInfoHeader;


    // Fill the file header with required info
    bmpFileHeader.bfType		= 'MB';
    bmpFileHeader.bfSize		= 0;
    bmpFileHeader.bfReserved1   = 0;
    bmpFileHeader.bfReserved2   = 0;
	bmpFileHeader.bfOffBits		= 14 + 40; // sizeof(bmpFileHeader) + sizeof(bmpInfoHeader);

    // Write the file header
	fwrite(&bmpFileHeader.bfType,        sizeof(bmpFileHeader.bfType),       1, outF);
	fwrite(&bmpFileHeader.bfSize,        sizeof(bmpFileHeader.bfSize),       1, outF);
	fwrite(&bmpFileHeader.bfReserved1,   sizeof(bmpFileHeader.bfReserved1),  1, outF);
	fwrite(&bmpFileHeader.bfReserved2,   sizeof(bmpFileHeader.bfReserved2),  1, outF);
	fwrite(&bmpFileHeader.bfOffBits,     sizeof(bmpFileHeader.bfOffBits),    1, outF);


    // Fill the info header with the required info
    bmpInfoHeader.biSize            = 40; // sizeof(bmpInfoHeader);
    bmpInfoHeader.biWidth           = _width;
    bmpInfoHeader.biHeight          = _height;
    bmpInfoHeader.biPlanes          = 1;
    bmpInfoHeader.biBitCount        = 24;
    bmpInfoHeader.biCompression     = 0L; // BI_RGB
    bmpInfoHeader.biSizeImage       = 0;
    bmpInfoHeader.biXPelsPerMeter   = 0;
    bmpInfoHeader.biYPelsPerMeter   = 0;
    bmpInfoHeader.biClrUsed         = 0;
    bmpInfoHeader.biClrImportant    = 0;

	// Write the info header
	fwrite(&bmpInfoHeader.biSize,            sizeof(bmpInfoHeader.biSize),           1, outF);
	fwrite(&bmpInfoHeader.biWidth,           sizeof(bmpInfoHeader.biWidth),          1, outF);
	fwrite(&bmpInfoHeader.biHeight,          sizeof(bmpInfoHeader.biHeight),         1, outF);
	fwrite(&bmpInfoHeader.biPlanes,          sizeof(bmpInfoHeader.biPlanes),         1, outF);
	fwrite(&bmpInfoHeader.biBitCount,        sizeof(bmpInfoHeader.biBitCount),       1, outF);
	fwrite(&bmpInfoHeader.biCompression,     sizeof(bmpInfoHeader.biCompression),    1, outF);
	fwrite(&bmpInfoHeader.biSizeImage,       sizeof(bmpInfoHeader.biSizeImage),      1, outF);
	fwrite(&bmpInfoHeader.biXPelsPerMeter,   sizeof(bmpInfoHeader.biXPelsPerMeter),  1, outF);
	fwrite(&bmpInfoHeader.biYPelsPerMeter,   sizeof(bmpInfoHeader.biYPelsPerMeter),  1, outF);
	fwrite(&bmpInfoHeader.biClrUsed,         sizeof(bmpInfoHeader.biClrUsed),        1, outF);
	fwrite(&bmpInfoHeader.biClrImportant,    sizeof(bmpInfoHeader.biClrImportant),   1, outF);


	// Write the pixel data. Every scanline should be dword aligned.

	// Padding bytes are necessary for 24 bit bitmaps.
	int numPaddingBytes = (_width * 3) % 4;
	if(numPaddingBytes) numPaddingBytes = 4 - numPaddingBytes;

    for( int y = _height-1; y >= 0; --y )
	{
		for( int x = 0; x < _width;  ++x )
		{
            // Get the pixel at the current position
            Pixel<> pixel;
            GetPixel(x, y, pixel);

            // Write it
			fwrite( &(pixel._b), sizeof(BYTE), 1, outF);
			fwrite( &(pixel._g), sizeof(BYTE), 1, outF);
            fwrite( &(pixel._r), sizeof(BYTE), 1, outF);
		}

		if( numPaddingBytes )
		{
			BYTE padByte = 0;

			for( int i = 0; i < numPaddingBytes; i++ )
				fwrite( &padByte, sizeof(BYTE), 1, outF);
		}
	}

    return true;
}
