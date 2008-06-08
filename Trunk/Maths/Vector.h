
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

#ifndef VECTOR_HEADER
#define VECTOR_HEADER

#ifdef _MSVC
#pragma warning( disable : 4201 )
#endif

template <class T>
class Vector
{
public:
    union
    {
	    struct
	    {
		    T x;
		    T y;
            T z;
	    };
	    T v[3];
    };

public:
    // Constructors
    explicit Vector();
    explicit Vector(const T &val);
    explicit Vector(
	    const T &xVal,
	    const T &yVal,
        const T &zVal);

    // Destructor
    ~Vector();

    // Copy Constructor / Assignment Operator
    Vector(const Vector &p);
    const Vector &operator =(const Vector &p);

    // Operators
    const bool operator ==(const Vector &p) const;
    void operator +=(const Vector &p);
    void operator -=(const Vector &p);
    void operator *=(const T &val);
    void operator *=(const Vector &p);
    const Vector operator +(const Vector &p) const;
    const Vector operator -(const Vector &p) const;
    const Vector operator *(const T &val) const;
    const Vector operator *(const Vector &p) const;
    const Vector operator -() const;   // Unary negation

    // Other functions
    void Set(const T &val);
    void Set(const T &xVal, const T &yVal, const T &zVal);

    const T Dot(const Vector &p) const;
    const Vector Cross(const Vector &p) const;
    const T Magnitude2() const;
    const T Magnitude() const;
    const T Normalize();
    const Vector Reflect(const Vector &n) const;
};

// Constructors

template <class T>
Vector<T>::Vector()
{
}

template <class T>
Vector<T>::Vector(const T &val)
{
    Set(val);
}

template <class T>
Vector<T>::Vector(
    const T &xVal,
    const T &yVal,
    const T &zVal)
{
    Set(xVal, yVal, zVal);
}

// Destructor
template <class T>
Vector<T>::~Vector()
{
}

// Copy Constructor / Assignment Operator

template <class T>
Vector<T>::Vector(const Vector &p)
{
    Set(p.x, p.y, p.z);
}

template <class T>
const Vector<T> &Vector<T>::operator =(const Vector &p)
{
    Set(p.x, p.y, p.z);
    return *this;
}

// Operators

template <class T>
const bool Vector<T>::operator ==(const Vector &p) const
{
    return(
	    x == p.x	&&
	    y == p.y	&&
        z == p.z    );
}

template <class T>
void Vector<T>::operator +=(const Vector &p)
{
    x += p.x;
    y += p.y;
    z += p.z;
}

template <class T>
void Vector<T>::operator -=(const Vector &p)
{
    x -= p.x;
    y -= p.y;
    z -= p.z;
}

template <class T>
void Vector<T>::operator *=(const T &val)
{
    x *= val;
    y *= val;
    z *= val;
}

template <class T>
void Vector<T>::operator *=(const Vector &p)
{
    x *= p.x;
    y *= p.y;
    z *= p.z;
}

template <class T>
const Vector<T> Vector<T>::operator +(const Vector &p) const
{
    return Vector(
	    x + p.x,
	    y + p.y,
        z + p.z );
}

template <class T>
const Vector<T> Vector<T>::operator -(const Vector &p) const
{
    return Vector(
	    x - p.x,
	    y - p.y,
        z - p.z );
}

template <class T>
const Vector<T> Vector<T>::operator *(const T &val) const
{
    return Vector(
	    x * val,
	    y * val,
        z * val );
}

template <class T>
const Vector<T> Vector<T>::operator *(const Vector &p) const
{
    return Vector(
        x * p.x,
        y * p.y,
        z * p.z );
}

template <class T>
const Vector<T> Vector<T>::operator -() const    // Unary negation
{
    return Vector( -x, -y, -z );
}

// Other functions

template <class T>
void Vector<T>::Set(const T &val)
{
    x = val;
    y = val;
    z = val;
}

template <class T>
void Vector<T>::Set(const T &xVal, const T &yVal, const T &zVal)
{
    x = xVal;
    y = yVal;
    z = zVal;
}

template <class T>
const T Vector<T>::Dot(const Vector &p) const
{
    return x*p.x + y*p.y + z*p.z;
}

template <class T>
const Vector<T> Vector<T>::Cross(const Vector &p) const
{
    return Vector(
        y * p.z - z * p.y,
        z * p.x - x * p.z,
        x * p.y - y * p.x );
}

template <class T>
const T Vector<T>::Magnitude2() const
{
    return (x*x + y*y + z*z);
}

// Defined in Vector.cpp
//const T Magnitude() const;

// Defined in Vector.cpp
//const T Normalize();

template <class T>
const Vector<T> Vector<T>::Reflect(const Vector &n) const
{
    return operator -(n * Dot(n) * 2);
}

#endif
