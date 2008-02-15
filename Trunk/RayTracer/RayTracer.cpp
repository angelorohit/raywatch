
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

#include "RayTracer.h"
#include "Primitive.h"
#include "Ray.h"
#include "Scene.h"
#include "Light.h"
#include "Image.h"
#include "Maths.h"
#include <iostream>

// Initialize the static members
bool RayTracer::_bRayTraceShadows = false;

// Constructor
RayTracer::RayTracer()
{
}

// Destructor
RayTracer::~RayTracer()
{
}

// Functions

const Color RayTracer::GetIllumination(const Ray &ray, const Scene &scene)
{
    // Note: We check the ray's generation against a doubled maxGenerations because
    //       we create an extra generation right here in this function below (while
    //       passing a new ray to Material's GetIllumination().
    if( ray.Generation() > (scene._maxRayGenerations * 2) )
        return Color( 0 );

    IntersectionInfo intersectionInfo;
    const Primitive *const pPrimitive = scene.FindClosestIntersection(ray, intersectionInfo);

    // The ray doesn't intersect anything, then return no color.
    if( !pPrimitive )
        return Color( 0 );

    // If the Primitive has a Light set to it, then return the Light's illumination
    if( pPrimitive->_pLight )
        return pPrimitive->_pLight->Illumination();

    // Return the illumination from the material
    return pPrimitive->_material.GetIllumination(
        Ray( intersectionInfo._point, ray.Direction(), ray ),
        pPrimitive->GetSurfaceNormal( intersectionInfo._point ),
        scene,
        intersectionInfo );
}

const bool RayTracer::Render(const Camera &camera, const Scene &scene, Image &image) const
{
    // RayTrace the Scene

    // For each pixel of the image
    for(int y=0; y < image.Height(); ++y)
    {
        for(int x=0; x < image.Width(); ++x)
        {
            const Vector<float> &rayOrigin = camera._position;

            Vector<float> rayDirection;
            {
                rayDirection.x = cos( Maths::DegToRad( Maths::InterpolateLinear( 90 + camera._hFov/2, 90 - camera._hFov/2, x / (float)image.Width() ) ) );
                rayDirection.y = cos( Maths::DegToRad( Maths::InterpolateLinear( 90 - camera._vFov/2, 90 + camera._vFov/2, y / (float)image.Height() ) ) );
                rayDirection.z = -sqrt( (2 - (rayDirection.x * rayDirection.x + rayDirection.y * rayDirection.y)) / 2 );
                rayDirection.Normalize();
            }

            // Get the illumination from the scene through this ray.
            const Color color = GetIllumination( Ray( rayOrigin, rayDirection, Ray::RootGeneration() ), scene );

            // Plot it.
            image.SetPixel(x, y, Pixel<float>(color.x, color.y, color.z, 1));
        }

        // Display the no. of the row just completed
        std::cout << ".";
    }

    return true;
}
