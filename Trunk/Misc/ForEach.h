
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

#ifndef FOREACH_HEADER
#define FOREACH_HEADER

// Constant iterator
#define FOR_EACH(IteratorName,ContainerType,ContainerName)                      \
    for(ContainerType::const_iterator IteratorName( ContainerName.begin() );    \
        IteratorName != ContainerName.end();                                    \
        ++IteratorName)

// Non-constant iterator
#define FOR_EACH_MUTABLE(IteratorName,ContainerType,ContainerName)      \
    for(ContainerType::iterator IteratorName( ContainerName.begin() );  \
        IteratorName != ContainerName.end();                            \
        ++IteratorName)

#endif
