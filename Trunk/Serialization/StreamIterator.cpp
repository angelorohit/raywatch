
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

#include "StreamIterator.h"
#include "CodeBlocks.h"
#include "Preprocessor.h"

// Constructor
StreamIterator::StreamIterator() :
    _buffer(),
    _cursor( _buffer.end() ),
    _cursorStack()
{
}

// Destructor
StreamIterator::~StreamIterator()
{
    Close();
}

// Functions
const bool StreamIterator::Open(std::istream &stream)
{
    Close();

    bool bRetVal = false;
    BEGIN_CODE_BLOCK
    {
        // See if the stream is usable
        if( !stream.good() )
            EXIT_CODE_BLOCK;

        // Read the entire file into the buffer
        std::copy( std::istreambuf_iterator<char>( stream ), std::istreambuf_iterator<char>(), std::back_inserter( _buffer ) );
        
        // Preprocess the buffer
        if( !Preprocessor::Process( _buffer ) )
            EXIT_CODE_BLOCK;

        Reset();

        // We're done initializing.
        bRetVal = true;
    }
    END_CODE_BLOCK

    if( !bRetVal )
        Close();

    return bRetVal;
}

void StreamIterator::Close()
{
    Buffer().swap( _buffer );
    _cursor = _buffer.end();
    CursorStack().swap( _cursorStack );
}

void StreamIterator::Reset()
{
    if( _buffer.empty() )
        return;

    _cursor = _buffer.begin();
    CursorStack().swap( _cursorStack );
}

const char StreamIterator::operator * () const
{
    return Eof()? (0): (*_cursor);
}

const bool StreamIterator::Eof() const
{
    return (_cursor == _buffer.end());
}

void StreamIterator::operator ++ ()
{
    if( !Eof() )
        ++_cursor;
}

void StreamIterator::SavePosition()
{
    if( !Eof() )
        _cursorStack.push_back( _cursor );
}

void StreamIterator::RestorePosition()
{
    if( _cursorStack.empty() )
        return;

    _cursor = _cursorStack.back();
    _cursorStack.pop_back();
}
