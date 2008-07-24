
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

#ifndef SAFEDELETE_HEADER
#define SAFEDELETE_HEADER

template <class T>
void SafeDeleteScalar(T *&pObject)
{
    delete pObject;
    pObject = 0;
}

template <class T>
void SafeDeleteArray(T *&pArray)
{
    delete [] pArray;
    pArray = 0;
}

#endif
