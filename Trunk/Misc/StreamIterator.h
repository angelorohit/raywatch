
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

#ifndef STREAM_HEADER
#define STREAM_HEADER

#include <string>
#include <vector>
#include <istream>

class StreamIterator
{
// Members
private:
    typedef std::string         Buffer;
    typedef Buffer::iterator    Cursor;
    typedef std::vector<Cursor> CursorStack;

    Buffer      _buffer;
    Cursor      _cursor;
    CursorStack _cursorStack;

public:
// Constructor
    explicit StreamIterator();
// Destructor
    ~StreamIterator();

private:
// Copy Constructor / Assignment Operator
    StreamIterator(const StreamIterator &);
    const StreamIterator &operator =(const StreamIterator &);

// Functions
public:
    const bool Open(std::istream &stream);
    void Close();
    void Reset();

    const char operator * () const; // Dereference iterator
    const bool Eof() const;         // Test for end of stream
    void operator ++ ();            // Advance iterator

    void SavePosition();
    void RestorePosition();
};

#endif
