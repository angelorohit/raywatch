
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

const bool Examples::Example2(const std::string &fileName)
{
    // Create a Scene
    Scene scene;
    scene._maxRayGenerations = 6;
    scene._ambientLight.Set( 0.1f );

    // Create textures
    Texture *pTexture1 = scene.LoadTexture( "Textures/Checks.jpg" );
    Texture *pTexture2 = scene.LoadTexture( "Textures/Strands.png" );
    if( !(pTexture1 && pTexture2) )
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
        pQuad->_material.SetTexture( pTexture1 );
        pQuad->_material.SetTextureScale( 0.25f );

        scene.AddPrimitive( pQuad );
    }
    // Sphere1
    {
        Sphere* pSphere = new Sphere();
        pSphere->SetCentre( Vector<float>(-0.25f, -0.4f, -4) );
        pSphere->SetRadius( 0.6f );
        pSphere->_material.SetColor(1, 1, 1);
        pSphere->_material.SetReflectivity( 0.25f );

        scene.AddPrimitive( pSphere );
    }
    // Sphere2
    {
        Sphere* pSphere = new Sphere();
        pSphere->SetCentre( Vector<float>(0.6f, -0.7f, -3.5f) );
        pSphere->SetRadius( 0.3f );
        pSphere->_material.SetColor(0.5f, 1, 0.5f);
        pSphere->_material.SetSpecularity( 1 );
        pSphere->_material.SetRoughness( 50 );

        pSphere->_material.SetRefractiveIndex( 1.05f );
        pSphere->_material.SetAbsorption( 3 );

        pSphere->_material.SetTexture( pTexture2 );

        scene.AddPrimitive( pSphere );
    }

    // Sphere3
    {
        Sphere* pSphere = new Sphere();
        pSphere->SetCentre( Vector<float>(-0.5f, -0.8f, -3) );
        pSphere->SetRadius( 0.2f );
        pSphere->_material.SetColor(1, 0.5f, 0.5f);
        pSphere->_material.SetSpecularity( 1 );
        pSphere->_material.SetRoughness( 50 );

        pSphere->_material.SetRefractiveIndex( 1.05f );
        pSphere->_material.SetAbsorption( 3 );

        pSphere->_material.SetTexture( pTexture2 );

        scene.AddPrimitive( pSphere );
    }

    // Light
    {
        // Area Light
        AreaLight *pLight   = new AreaLight();
        pLight->SetColor( Color( 1, 1, 1 ) );
        pLight->SetIntensity( 1 );
        pLight->SetRange( 4 );
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
        pLight->SetRange( 4 );
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
