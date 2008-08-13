
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

#ifndef TOKENSTREAM_HEADER
#define TOKENSTREAM_HEADER

#include "StreamIterator.h"

class TokenStream
{
// Members
private:
    StreamIterator _streamIterator;

public:
// Constructor
    explicit TokenStream();
// Destructor
    ~TokenStream();

private:
// Copy Constructor / Assignment Operator
    TokenStream(const TokenStream &);
    const TokenStream &operator =(const TokenStream &);

// Functions
private:
    static const bool IsContained(const char character, const std::string &characterSet);

public:
    const bool Open(std::istream &stream);
    void Close();

    const bool ReadToken(std::string &token, const std::string &delimiterSet, const bool bTrimWhitespaces = true);
    const bool PeekToken(std::string &token, const std::string &delimiterSet, const bool bTrimWhitespaces = true);

    const bool ReadToken(std::string &token, const std::size_t tokenLength);
    const bool PeekToken(std::string &token, const std::size_t tokenLength);
};

#endif
