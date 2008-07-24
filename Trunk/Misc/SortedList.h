
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

#ifndef SORTEDLIST_HEADER
#define SORTEDLIST_HEADER

#include <vector>

template <class Type, class Compare>
class SortedList
{
public:
    typedef std::vector<Type> ContainerType;

// Members
private:
    ContainerType   _container;
    Compare         _compare;

public:
// Constructor
    explicit SortedList() :
        _container(),
        _compare()
    {
    }

// Destructor
    ~SortedList()
    {
        clear();
    }

private:
// Copy Constructor / Assignment Operator
    SortedList(const SortedList &);
    const SortedList &operator =(const SortedList &);

// Functions
public:
    void push(const Type &value)
    {
        // Find the appropriate position to insert the value
        typename ContainerType::iterator itr;
        for(itr = _container.begin(); itr != _container.end(); ++itr)
            if( _compare( *itr, value ) )
                break;

        _container.insert( itr, value );
    }

    void clear()
    {
        ContainerType().swap( _container );
    }

    const bool empty() const
    {
        return _container.empty();
    }

    typename ContainerType::const_iterator begin() const
    {
        return _container.begin();
    }

    typename ContainerType::const_iterator end() const
    {
        return _container.end();
    }

    const Type &top() const
    {
        return _container.front();
    }
};

#endif
