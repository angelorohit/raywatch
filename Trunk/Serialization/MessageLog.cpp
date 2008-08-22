
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

#include "MessageLog.h"
#include <cassert>

// Constructor
MessageLog::MessageLog() :
    _messages(),
    _currentMessage(),
    _pLineNumberProvider(0)
{
}

// Destructor
MessageLog::~MessageLog()
{
}

// Functions
const int MessageLog::CurrentLineNumber() const
{
    return (_pLineNumberProvider)?
        (_pLineNumberProvider->ProvideLineNumber()):
        (-1);
}

void MessageLog::SetLineNumberProvider(const LineNumberProvider *const pProvider)
{
    assert( pProvider );
    _pLineNumberProvider = pProvider;
}

const MessageLog::MessageList &MessageLog::Messages() const
{
    return _messages;
}

// For streaming strings
MessageLog &MessageLog::operator << (const std::string &value)
{
    _currentMessage.append( value );
    return *this;
}

MessageLog &MessageLog::operator << (const char *const value)
{
    return operator <<( std::string( value ) );
}

// For streaming other data types
MessageLog &MessageLog::operator << (const std::size_t value)
{
    _currentMessage.append( Utility::String::ToString( value ) );
    return *this;
}

MessageLog &MessageLog::operator << (const int value)
{
    _currentMessage.append( Utility::String::ToString( value ) );
    return *this;
}

MessageLog &MessageLog::operator << (const float value)
{
    _currentMessage.append( Utility::String::ToString( value ) );
    return *this;
}

// For streaming a manipulator
MessageLog &MessageLog::operator << (Manipulator func)
{
    func( *this );
    return *this;
}

// Manipulators
void endl(MessageLog &log)
{
    MessageLog::Message message( log.CurrentLineNumber(), log._currentMessage );
    log._messages.push_back( message );
    log._currentMessage.clear();
}
