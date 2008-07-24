
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

#ifndef UTILITY_HEADER
#define UTILITY_HEADER

// Includes
#include <string>
#include <sstream>

namespace Utility
{
    namespace String
    {
        // Functions
        const std::string &WhitespaceCharSet();

        void TrimWhiteSpaces(std::string &str);
        const bool ContainsWhiteSpaces(const std::string &str);
        const int CaseInsensitiveCompare(const std::string &str1, const std::string &str2);
        const std::string ToUpper(const std::string &str);

        // Templates

        template<class T>
        const std::string ToString(const T &val)
        {
            std::stringstream strStream;
            strStream << val;
            return strStream.str();
        }

        template<class T>
        const bool FromString(T &val, const std::string &str)
        {
            std::istringstream iss(str);
            return !(iss >> std::dec >> val).fail();
        }

    } // String

} // Utility

#endif

