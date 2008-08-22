
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

#ifndef MESSAGELOG_HEADER
#define MESSAGELOG_HEADER

#include "LineNumberProvider.h"
#include "Utility.h"
#include <string>
#include <list>

class MessageLog
{
// Manipulators
    friend void endl(MessageLog &);

// Types
public:
    typedef std::pair<int, std::string> Message;    // (line number, message text)
    typedef std::list<Message>          MessageList;

    typedef void (*Manipulator)(MessageLog &);

// Members
private:
    MessageList                  _messages;
    std::string                  _currentMessage;
    const LineNumberProvider    *_pLineNumberProvider;

public:
// Constructor / Destructor
    explicit MessageLog();
    ~MessageLog();

private:
// Copy Constructor / Assignment Operator
    MessageLog(const MessageLog &);
    const MessageLog &operator =(const MessageLog &);

// Functions
private:
    const int CurrentLineNumber() const;

public:
    void SetLineNumberProvider(const LineNumberProvider *const pProvider);
    const MessageList &Messages() const;

    // For streaming strings
    MessageLog &operator << (const std::string &value);
    MessageLog &operator << (const char *const value);

    // For streaming other data types
    MessageLog &operator << (const std::size_t  value);
    MessageLog &operator << (const int          value);
    MessageLog &operator << (const float        value);

    // For streaming a manipulator
    MessageLog &operator << (Manipulator func);
};

// Manipulator Prototypes
// Note: Manipulators are defined in MessageLog.cpp
void endl(MessageLog &);

#endif
