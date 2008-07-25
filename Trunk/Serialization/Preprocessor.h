
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

#ifndef PREPROCESSOR_HEADER
#define PREPROCESSOR_HEADER

#include "ForEach.h"
#include "SortedList.h"
#include <string>
#include <deque>
#include <algorithm>

class Preprocessor
{
private:
    template <class T>
    struct LessSize
    {
        const bool operator () (const T &left, const T &right) const
        {
            return ( left.size() < right.size() );
        }
    };

    class KeywordCollection
    {
    public:
        typedef SortedList< std::string, LessSize<std::string> > KeywordList;
    private:
        KeywordList _keywords;

    public:
        const bool RegisterKeyword(const std::string &keyword)
        {
            if( std::find( _keywords.begin(), _keywords.end(), keyword ) != _keywords.end() )
                return false;

            _keywords.push( keyword );

            return true;
        }

        const KeywordList &Keywords() const
        {
            return _keywords;
        }

        const size_t LongestKeywordSize() const
        {
            return (_keywords.empty())? (0): (_keywords.top().size());
        }
    };

    class CharacterHistory
    {
    private:
        typedef std::deque<char> CharacterQueue;

        CharacterQueue           _history;
        const KeywordCollection &_keywordCollection;

    private:
        // Private assignment operator
        const CharacterHistory &operator = (const CharacterHistory &);

    public:
        explicit CharacterHistory(const KeywordCollection &keywordCollection) :
            _history( keywordCollection.LongestKeywordSize(), 0 ),
            _keywordCollection( keywordCollection )
        {
        }

        void Clear()
        {
            std::fill( _history.begin(), _history.end(), 0 );
        }

        void Visit(const char character)
        {
            _history.push_back( character );
            _history.pop_front();
        }

        const std::string *const VisitedKeyword() const
        {
            // See if we've just completed visiting a whole keyword
            const std::string *pVisitedKeyword = 0;
            FOR_EACH( itr, KeywordCollection::KeywordList::ContainerType, _keywordCollection.Keywords() )
            {
                if( std::equal( (*itr).rbegin(), (*itr).rend(), _history.rbegin() ) )
                {
                    pVisitedKeyword = &(*itr);
                    break;
                }
            }

            return pVisitedKeyword;
        }
    };

    static const bool RemoveComments(std::string &buffer)
    {
        // Comment definitions
        const std::string cPlusPlusStyleComment( "//" );
        const std::string cStyleCommentStart( "/*" );
        const std::string cStyleCommentEnd( "*/" );

        // Our comment keywords
        KeywordCollection comments;
        comments.RegisterKeyword( cPlusPlusStyleComment );
        comments.RegisterKeyword( cStyleCommentStart );

        CharacterHistory charHistory( comments );
        for(std::string::iterator itr = buffer.begin(); itr != buffer.end(); )
        {
            charHistory.Visit( *itr++ );

            // See if we've visited any keyword
            const std::string *const pVisitedKeyword = charHistory.VisitedKeyword();
            // If we haven't visited any keyword, then continue, visit the next character.
            if( !pVisitedKeyword )
                continue;

            // We've visited a keyword; clear the history
            charHistory.Clear();
            // Get the position where the keyword begins
            std::string::iterator startItr = itr - pVisitedKeyword->size();

            // Do the required processing for the keyword visited.

            if( pVisitedKeyword->compare( cPlusPlusStyleComment ) == 0 )
            {
                // Fill with spaces upto the end of this line
                std::string::iterator endItr = std::find( itr, buffer.end(), '\n' );
                std::fill( startItr, endItr, ' ' );
                
                continue;
            }

            if( pVisitedKeyword->compare( cStyleCommentStart ) == 0 )
            {
                // Find the end of this comment
                std::string::iterator endItr = std::find_first_of(itr, buffer.end(), cStyleCommentEnd.begin(), cStyleCommentEnd.end());
                if( endItr != buffer.end() )
                    endItr += cStyleCommentEnd.size();

                // Fill the whole comment with spaces
                std::fill( startItr, endItr, ' ' );
                
                continue;
            }
        }

        return true;
    }

public:
    static const bool Process(std::string &buffer)
    {
        if( !RemoveComments( buffer ) )
            return false;

        return true;
    }
};

#endif
