
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

#ifndef PIXEL_HEADER
#define PIXEL_HEADER

template <class T = unsigned char>
class Pixel
{
// Members
public:
    T   _r;
    T   _g;
    T   _b;
    T   _a;

public:
// Constructors
    explicit Pixel();
    explicit Pixel(
        const T &r,
        const T &g,
        const T &b,
        const T &a );

// Copy constructor / Assignment operator
    Pixel(const Pixel &val);
    const Pixel &operator = (const Pixel &val);

// Operators
    const Pixel operator +(const Pixel &p) const;
    const Pixel operator -(const Pixel &p) const;
    const Pixel operator *(const T &val) const;

// Functions
    void Set(
        const T &r,
        const T &g,
        const T &b,
        const T &a );
};

// Constructors
template <class T>
Pixel<T>::Pixel()
{
}

template <class T>
Pixel<T>::Pixel(
    const T &r,
    const T &g,
    const T &b,
    const T &a )
{
    Set( r, g, b, a );
}

// Copy constructor / Assignment operator
template <class T>
Pixel<T>::Pixel(const Pixel &val)
{
    Set( val._r, val._g, val._b, val._a );
}

template <class T>
const Pixel<T> &Pixel<T>::operator = (const Pixel &val)
{
    Set( val._r, val._g, val._b, val._a );
    return *this;
}

// Operators
template <class T>
const Pixel<T> Pixel<T>::operator +(const Pixel &p) const
{
    return Pixel(
	    _r + p._r,
	    _g + p._g,
        _b + p._b,
        _a + p._a);
}

template <class T>
const Pixel<T> Pixel<T>::operator -(const Pixel &p) const
{
    return Pixel(
	    _r - p._r,
	    _g - p._g,
        _b - p._b,
        _a - p._a);
}

template <class T>
const Pixel<T> Pixel<T>::operator *(const T &val) const
{
    return Pixel(
	    _r * val,
	    _g * val,
        _b * val,
        _a * val );
}

// Functions
template <class T>
void Pixel<T>::Set(
    const T &r,
    const T &g,
    const T &b,
    const T &a )
{
	_r = r;
	_g = g;
	_b = b;
	_a = a;
}

#endif
