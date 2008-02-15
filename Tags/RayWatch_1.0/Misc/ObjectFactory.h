
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

#ifndef OBJECTFACTORY_HEADER
#define OBJECTFACTORY_HEADER

#include <map>
#include <string>

// Macro to register a Type with the Object Factory
#define ObjectFactory_Register(_AbstractProduct_,_ConcreteProduct_)                                         \
    ObjectFactory<_AbstractProduct_>::AssocMap ObjectFactory<_AbstractProduct_>::_associations;             \
    namespace                                                                                               \
    {                                                                                                       \
        _AbstractProduct_ *const Create() { return new _ConcreteProduct_(); }                               \
        const bool bRegistered = ObjectFactory<_AbstractProduct_>::Register( #_ConcreteProduct_, Create );  \
    }

template
<
    class AbstractProduct,
    typename IdentifierType = std::string,
    typename ProductCreator = AbstractProduct *const (*) ()
>
class ObjectFactory
{
// Members
private:
    typedef std::map<IdentifierType, ProductCreator> AssocMap;
    static AssocMap _associations;

// Monostate related
private:
    // Constructor / Destructor
    explicit ObjectFactory();
    ~ObjectFactory();

    // Copy Constructor / Assignment Operator
    ObjectFactory(const ObjectFactory &);
    const ObjectFactory &operator =(const ObjectFactory &);

// Functions
public:
    static const bool Register(const IdentifierType &identifier, ProductCreator creator)
    {
        return _associations.insert( AssocMap::value_type( identifier, creator ) ).second;
    }

    static AbstractProduct *const Create(const IdentifierType &identifier)
    {
        AssocMap::const_iterator itr = _associations.find( identifier );

        if( itr != _associations.end() )
            return (itr->second)();

        return 0;
    }
};

#endif
