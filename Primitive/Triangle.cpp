
//  RayWatch - A simple cross-platform RayTracer.
//  Copyright (C) 2008
//      Angelo Rohit Joseph Pulikotil,
//      Francis Xavier Joseph Pulikotil
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

#include "Triangle.h"
#include "Ray.h"
#include "Maths.h"
#include "ObjectFactory.h"

// Register with the ObjectFactory
ObjectFactory_Register(Serializable, Triangle);

// Constructor
Triangle::Triangle()
{
}

// Destructor
Triangle::~Triangle()
{
}

// Functions

// Primitive's functions
const bool Triangle::Intersects(const Ray &ray, IntersectionInfo &intersectionInfo) const
{
    const float d = ray.Direction().Dot( _surfaceNormal );
    if( Maths::IsApproxEqual( d, 0 ) )
        return false;

    intersectionInfo._dist = -(ray.Origin() - _v2).Dot( _surfaceNormal ) / d;
    if( intersectionInfo._dist < 0.01f )
        return false;

    intersectionInfo._point = ray.Origin() + ray.Direction() * intersectionInfo._dist;

    if( (intersectionInfo._point - _v1).Dot( _edge1Normal ) > 0   ||
        (intersectionInfo._point - _v2).Dot( _edge2Normal ) > 0   ||
        (intersectionInfo._point - _v3).Dot( _edge3Normal ) > 0   )
        return false;

    intersectionInfo._bOnEntry = (d < 0);

    return true;
}

const bool Triangle::Intersects(const Ray &ray, float &intersectionDist) const
{
    // As an optimization, the code above can be duplicated here with
    // only the necessary calculations; for now we reuse the above code

    IntersectionInfo intersectionInfo;
    const bool bIntersects = Intersects( ray, intersectionInfo );

    intersectionDist = intersectionInfo._dist;
    return bIntersects;
}

const Vector<float> Triangle::GetSurfaceNormal(const Vector<float> &/*position*/) const
{
    return _surfaceNormal;
}

void Triangle::SetVertices(const Vector<float> &v1, const Vector<float> &v2, const Vector<float> &v3)
{
    _v1 = v1;
    _v2 = v2;
    _v3 = v3;

    _surfaceNormal = (_v3 - _v2).Cross( v1 - v2 );
    _surfaceNormal.Normalize();

    _edge1Normal = (_v2 - _v1).Cross( _surfaceNormal );
    _edge1Normal.Normalize();

    _edge2Normal = (_v3 - _v2).Cross( _surfaceNormal );
    _edge2Normal.Normalize();

    _edge3Normal = (_v1 - _v3).Cross( _surfaceNormal );
    _edge3Normal.Normalize();
}

// Serializable's functions
const bool Triangle::Read(std::istream &stream)
{
    // Read the base
    if( !ReadHeader( stream, "Primitive" ) || !Primitive::Read( stream ) )
        return false;

    Vector<float> vertex1, vertex2, vertex3;
    if( !ReadVariable( stream, "vertex1", vertex1 ) ||
        !ReadVariable( stream, "vertex2", vertex2 ) ||
        !ReadVariable( stream, "vertex3", vertex3 ) )
        return false;

    SetVertices( vertex1, vertex2, vertex3 );

    if( !ReadFooter( stream, "Triangle" ) )
        return false;

    return true;
}

const bool Triangle::Write(std::ostream &stream) const
{
    if( !WriteHeader( stream, "Triangle" ) )
        return false;

    Indent();
    {
        // Write the base
        if( !Primitive::Write( stream ) )
            return false;

        if( !WriteVariable( stream, "vertex1", _v1 )    ||
            !WriteVariable( stream, "vertex2", _v2 )    ||
            !WriteVariable( stream, "vertex3", _v3 )    )
            return false;
    }
    Unindent();

    if( !WriteFooter( stream, "Triangle" ) )
        return false;

    return true;
}
