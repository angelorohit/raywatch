
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

#include "Examples.h"

#include "Scene.h"
#include "Quad.h"
#include "Sphere.h"
#include "Triangle.h"
#include "PointLight.h"
#include "AreaLight.h"
#include "Camera.h"
#include "Image.h"
#include "RayTracer.h"
#include <fstream>

const bool Examples::Example1(const std::string &fileName)
{
    // Create a Scene
    Scene scene;
    scene._maxRayGenerations = 3;
    scene._ambientLight.Set( 0 );

    // Create a texture
    Texture *pTexture = scene.LoadTexture( "Media/Textures/Checks.jpg" );
    if( !pTexture )
    {
        std::cout << "Error: Failed to create Scene" << std::endl;
        return false;
    }

    // Bottom Quad
    {
        Quad *pQuad = new Quad();
        pQuad->SetVertices(
            Vector<float>( -2, -1, -6),
            Vector<float>( -2, -1, -2),
            Vector<float>(  2, -1, -2) );
        pQuad->_material.SetColor(1, 1, 1);
        pQuad->_material.SetTexture( pTexture );
        pQuad->_material.SetTextureScale( 0.25f );

        scene.AddPrimitive( pQuad );
    }
    // Sphere1
    {
        Sphere* pSphere = new Sphere();
        pSphere->SetCentre( Vector<float>(0.5f, -0.75f, -3) );
        pSphere->SetRadius( 0.25f );
        pSphere->_material.SetColor(1, 1, 1);
        pSphere->_material.SetReflectivity( 1 );

        scene.AddPrimitive( pSphere );
    }
    // Sphere2
    {
        Sphere* pSphere = new Sphere();
        pSphere->SetCentre( Vector<float>(-0.4f, -0.6f, -4) );
        pSphere->SetRadius( 0.4f );
        pSphere->_material.SetColor(1, 0, 0);
        pSphere->_material.SetReflectivity( 0.25f );
        pSphere->_material.SetSpecularity( 0.5f );
        pSphere->_material.SetRoughness( 20 );
        pSphere->_material.SetFuzzyReflectionRadius( 0.2f );
        pSphere->_material.SetFuzzyReflectionSamples( 50 );

        scene.AddPrimitive( pSphere );
    }

    // Light
    {
        // Area Light
        AreaLight *pLight   = new AreaLight();
        pLight->SetColor( Color( 1, 1, 1 ) );
        pLight->SetIntensity( 1 );
        pLight->SetRange( 3 );
        pLight->SetRectangularArea(
            Vector<float>(  0.25f, 1, -3),
            Vector<float>(  0.25f, 1, -3.5),
            Vector<float>(  0.75f, 1, -3.5),
            5, 5 );

        scene.AddLight( pLight );

        // Area Light Quad
        Quad *pQuad = new Quad();
        pQuad->SetVertices(
            Vector<float>(  0.25f, 1, -3),
            Vector<float>(  0.25f, 1, -3.5),
            Vector<float>(  0.75f, 1, -3.5) );
        pQuad->_pLight = pLight;

        scene.AddPrimitive( pQuad );
    }

    // Light
    {
        // Area Light
        AreaLight *pLight   = new AreaLight();
        pLight->SetColor( Color( 1, 1, 1 ) );
        pLight->SetIntensity( 1 );
        pLight->SetRange( 3 );
        pLight->SetRectangularArea(
            Vector<float>( -0.75f, 1, -3),
            Vector<float>( -0.75f, 1, -3.5),
            Vector<float>( -0.25f, 1, -3.5),
            5, 5 );

        scene.AddLight( pLight );

        // Area Light Quad
        Quad *pQuad = new Quad();
        pQuad->SetVertices(
            Vector<float>( -0.75f, 1, -3),
            Vector<float>( -0.75f, 1, -3.5),
            Vector<float>( -0.25f, 1, -3.5) );
        pQuad->_pLight = pLight;

        scene.AddPrimitive( pQuad );
    }

    // Create the file
    std::fstream stream;
    stream.open( fileName.c_str(), std::ios_base::out | std::ios_base::trunc );
    if( !stream.is_open() )
    {
        std::cout << "Error: Failed to create file: " << fileName << std::endl;
        return false;
    }

    // Serialize the scene
    if( !scene.Write( stream ) )
    {
        std::cout << "Error: Failed to write Scene" << std::endl;
        return false;
    }

    return true;
}
