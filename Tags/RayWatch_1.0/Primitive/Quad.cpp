
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

#include "Quad.h"
#include "Ray.h"
#include "Maths.h"
#include "ObjectFactory.h"

// Register with the ObjectFactory
ObjectFactory_Register(Serializable, Quad);

// Constructor
Quad::Quad()
{
}

// Destructor
Quad::~Quad()
{
}

// Functions

// Primitive's functions
const bool Quad::Intersects(const Ray &ray, IntersectionInfo &intersectionInfo) const
{
    const float d = ray.Direction().Dot( _surfaceNormal );
    if( Maths::IsApproxEqual( d, 0 ) )
        return false;

    intersectionInfo._dist = -(ray.Origin() - _topLeft).Dot( _surfaceNormal ) / d;
    if( intersectionInfo._dist < 0.01f )
        return false;

    intersectionInfo._point = ray.Origin() + ray.Direction() * intersectionInfo._dist;

    intersectionInfo._tU = (intersectionInfo._point - _topLeft).Dot( _horizontalNormal );
    if( intersectionInfo._tU < 0 || intersectionInfo._tU > _width )
        return false;

    intersectionInfo._tV = (intersectionInfo._point - _topLeft).Dot( _verticalNormal );
    if( intersectionInfo._tV < 0 || intersectionInfo._tV > _height )
        return false;

    intersectionInfo._bOnEntry = (d < 0);

    return true;
}

const bool Quad::Intersects(const Ray &ray, float &intersectionDist) const
{
    // As an optimization, the code above can be duplicated here with
    // only the necessary calculations; for now we reuse the above code

    IntersectionInfo intersectionInfo;
    const bool bIntersects = Intersects( ray, intersectionInfo );

    intersectionDist = intersectionInfo._dist;
    return bIntersects;
}

const Vector<float> Quad::GetSurfaceNormal(const Vector<float> &/*position*/) const
{
    return _surfaceNormal;
}

void Quad::SetVertices(const Vector<float> &v1, const Vector<float> &v2, const Vector<float> &v3)
{
    _topLeft = v1;
    _v2      = v2;  // Only for Serializing later
    _v3      = v3;  // Only for Serializing later

    _surfaceNormal = (v3 - v2).Cross( v1 - v2 );
    _surfaceNormal.Normalize();

    _horizontalNormal = (v1 - v2).Cross( _surfaceNormal );
    _horizontalNormal.Normalize();

    _verticalNormal = (v3 - v2).Cross( _surfaceNormal );
    _verticalNormal.Normalize();

    _width = (v3 - v2).Magnitude();
    _height = (v2 - v1).Magnitude();
}

// Serializable's functions
const bool Quad::Read(std::istream &stream)
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

    if( !ReadFooter( stream, "Quad" ) )
        return false;

    return true;
}

const bool Quad::Write(std::ostream &stream) const
{
    if( !WriteHeader( stream, "Quad" ) )
        return false;

    Indent();
    {
        // Write the base
        if( !Primitive::Write( stream ) )
            return false;

        if( !WriteVariable( stream, "vertex1", _topLeft )   ||
            !WriteVariable( stream, "vertex2", _v2 )        ||
            !WriteVariable( stream, "vertex3", _v3 )        )
            return false;
    }
    Unindent();

    if( !WriteFooter( stream, "Quad" ) )
        return false;

    return true;
}
