
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

#include "Sphere.h"
#include "Ray.h"
#include "Maths.h"
#include <math.h>

// Constructor
Sphere::Sphere() :
    _centre(0),
    _radius(0),
    _oneOverRadius(0)
{
}

// Destructor
Sphere::~Sphere()
{
}

// Functions

void Sphere::SetCentre(const Vector<float> &centre)
{
    _centre = centre;
}

void Sphere::SetRadius(const float &radius)
{
    _radius = radius;
    _oneOverRadius = 1 / _radius;
}

const float Sphere::GetLongitudeAt(const Vector<float> &position) const
{
    return acos( Maths::Bound<float>( (position.x - _centre.x) * _oneOverRadius, -1, 1 ) ) / Maths::Pi;
}

const float Sphere::GetLatitudeAt(const Vector<float> &position) const
{
    return acos( Maths::Bound<float>( (position.y - _centre.y) * _oneOverRadius, -1, 1 ) ) / Maths::Pi;
}

// Primitive's functions

const bool Sphere::Intersects(const Ray &ray, IntersectionInfo &intersectionInfo) const
{
    const Vector<float> rayToCentre = _centre - ray.Origin();
    const float v = rayToCentre.Dot( ray.Direction() );
    const float d2 = _radius*_radius - (rayToCentre.Magnitude2() - v*v);

    if( d2 >= 0 )
    {
        const float d = sqrt( d2 );

        intersectionInfo._dist = v - d;
        if( intersectionInfo._dist > 0.01f )
        {
            intersectionInfo._point = ray.Origin() + ray.Direction() * intersectionInfo._dist;
            intersectionInfo._tU = GetLongitudeAt( intersectionInfo._point );
            intersectionInfo._tV = GetLatitudeAt( intersectionInfo._point );
            intersectionInfo._bOnEntry = true;
            return true;
        }

        intersectionInfo._dist = v + d;
        if( intersectionInfo._dist > 0.01f )
        {
            intersectionInfo._point = ray.Origin() + ray.Direction() * intersectionInfo._dist;
            intersectionInfo._tU = GetLongitudeAt( intersectionInfo._point );
            intersectionInfo._tV = GetLatitudeAt( intersectionInfo._point );
            intersectionInfo._bOnEntry = false;
            return true;
        }
    }

    return false;
}

const bool Sphere::Intersects(const Ray &ray, float &intersectionDist) const
{
    // As an optimization, the code above can be duplicated here with
    // only the necessary calculations; for now we reuse the above code

    IntersectionInfo intersectionInfo;
    const bool bIntersects = Intersects( ray, intersectionInfo );

    intersectionDist = intersectionInfo._dist;
    return bIntersects;
}

const Vector<float> Sphere::GetSurfaceNormal(const Vector<float> &position) const
{
    return (position - _centre) * _oneOverRadius;
}
