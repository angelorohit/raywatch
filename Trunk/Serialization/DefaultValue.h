
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

#ifndef DEFAULTVALUE_HEADER
#define DEFAULTVALUE_HEADER

template <class T>
class DefaultValue
{
// Members
private:
    const T     _value;
    const bool  _bExists;

public:
// Constructors
    explicit DefaultValue();
    DefaultValue(const T &value);   // Allow implicit conversions from T

// Note: When binding an rValue of class type to a reference, the C++ standard requires
//       the copy constructor of the class to be accessible, even though it might not be
//       used. So to prevent compiler errors, make copy constructor declarations public,
//       but don't define them so that their use will atleast trigger a linker error.
public:
// Copy Constructor
    DefaultValue(const DefaultValue &);
private:
// Assignment Operator
    const DefaultValue &operator =(const DefaultValue &);

// Functions
public:
    const bool Exists() const;  // Whether the default value exists or not
    const T &Get() const;       // Get the default value
};

// Constructors
template <class T>
DefaultValue<T>::DefaultValue() :
    _value(),
    _bExists( false )
{
}

template <class T>
DefaultValue<T>::DefaultValue(const T &value) :
    _value( value ),
    _bExists( true )
{
}

// Functions
template <class T>
const bool DefaultValue<T>::Exists() const
{
    return _bExists;
}

template <class T>
const T &DefaultValue<T>::Get() const
{
    return _value;
}

#endif
