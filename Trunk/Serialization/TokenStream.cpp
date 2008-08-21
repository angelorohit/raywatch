
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

#include "TokenStream.h"
#include "Utility.h"
#include <algorithm>

// Constructor
TokenStream::TokenStream() :
    _streamIterator()
{
}

// Destructor
TokenStream::~TokenStream()
{
    Close();
}

// Functions
const bool TokenStream::IsContained(const char character, const std::string &characterSet)
{
    return ( std::find( characterSet.begin(), characterSet.end(), character ) != characterSet.end() );
}

const bool TokenStream::Open(std::istream &stream)
{
    return _streamIterator.Open( stream );
}

void TokenStream::Close()
{
    _streamIterator.Close();
}

const int TokenStream::LineNumber() const
{
    return _streamIterator.LineNumber();
}

const bool TokenStream::ReadToken(std::string &token, const std::string &delimiterSet, const bool bTrimWhitespaces, const bool bRemoveDelimiter)
{
    token.clear();

    // Read all white spaces before the token
    while( !_streamIterator.Eof() && IsContained(*_streamIterator, Utility::String::WhitespaceCharSet()) )
    {
        if( !bTrimWhitespaces )
            token.push_back( *_streamIterator );
        ++_streamIterator;
    }

    // Read upto a delimiter
    while( !_streamIterator.Eof() && !IsContained(*_streamIterator, delimiterSet) )
    {
        token.push_back( *_streamIterator );
        ++_streamIterator;
    }

    if( bTrimWhitespaces )
        Utility::String::TrimWhiteSpaces( token );

    // Eat-up the delimiter if required
    if( bRemoveDelimiter )
        ++_streamIterator;

    return (token.size() > 0);
}

const bool TokenStream::PeekToken(std::string &token, const std::string &delimiterSet, const bool bTrimWhitespaces)
{
    _streamIterator.SavePosition();
    const bool bRetVal = ReadToken( token, delimiterSet, bTrimWhitespaces );
    _streamIterator.RestorePosition();
    return bRetVal;
}

const bool TokenStream::ReadToken(std::string &token, const std::size_t tokenLength)
{
    token.clear();

    // Skip all white spaces before the token
    while( !_streamIterator.Eof() && IsContained(*_streamIterator, Utility::String::WhitespaceCharSet()) )
        ++_streamIterator;

    // Read upto tokenLength number of characters or upto a white space.
    while( !_streamIterator.Eof() && (token.size() < tokenLength) && !IsContained(*_streamIterator, Utility::String::WhitespaceCharSet()) )
    {
        token.push_back( *_streamIterator );
        ++_streamIterator;
    }

    return (token.size() >= tokenLength);
}

const bool TokenStream::PeekToken(std::string &token, const std::size_t tokenLength)
{
    _streamIterator.SavePosition();
    const bool bRetVal = ReadToken( token, tokenLength );
    _streamIterator.RestorePosition();
    return bRetVal;
}
