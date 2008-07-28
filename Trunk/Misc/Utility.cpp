
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

#include "Utility.h"
#include <algorithm>

namespace Utility
{
    namespace String
    {
        // Functions

        const std::string &WhitespaceCharSet()
        {
            static const std::string whitespaceCharSet( " \r\n\t" );
            return whitespaceCharSet;
        }

        void TrimWhiteSpaces(std::string &str)
        {
            size_t startPos, endPos;

            if( (startPos = str.find_first_not_of( WhitespaceCharSet() )) != std::string::npos &&
                (endPos   = str.find_last_not_of ( WhitespaceCharSet() )) != std::string::npos )
            {
                str = str.substr( startPos, (endPos - startPos + 1) );
            }
            else
            {
                str.clear();
            }
        }

        const bool ContainsWhiteSpaces(const std::string &str)
        {
            if( str.find_first_of( WhitespaceCharSet() ) != std::string::npos )
                return true;

            return false;
        }

        const int CaseInsensitiveCompare(const std::string &str1, const std::string &str2)
        {
            std::string::const_iterator itr1 = str1.begin();
            std::string::const_iterator itr2 = str2.begin();

            // Stop when either string's end has been reached
            while( (itr1 != str1.end()) && (itr2 != str2.end()) ) 
            {
                if( std::toupper(*itr1) != std::toupper(*itr2) )	// Letters differ?
                    // Return -1 to indicate smaller than, 1 otherwise
                    return (std::toupper(*itr1) < std::toupper(*itr2)) ? -1 : 1;

                // Proceed to the next character in each string
                ++itr1;
                ++itr2;
            }

            // Get the sizes of the strings
            size_t size1 = str1.size();
            size_t size2 = str2.size();

            // Return -1, 0 or 1 according to the string lengths
            if( size1 == size2 )
                return 0;

            return (size1 < size2) ? -1 : 1;
        }

        void ToUpper(std::string &str)
        {
            std::transform( str.begin(), str.end(), str.begin(), ToUpper<char> );
        }

        void ToLower(std::string &str)
        {
            std::transform( str.begin(), str.end(), str.begin(), ToLower<char> );
        }

    } // String

} // Utility

