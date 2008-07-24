
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
#include "Serializer.h"
#include <fstream>
#include <iostream>

const bool Examples::CornellBox(const std::string &fileName)
{
    // Create a Scene
    Scene scene;
    scene._maxRayGenerations = 3;
    scene._ambientLight.Set( 0.1f );

    // Back Quad
    {
        Quad *pQuad = new Quad();
        pQuad->SetVertices(
            Vector<float>( -1,  1, -4),
            Vector<float>( -1, -1, -4),
            Vector<float>(  1, -1, -4) );
        pQuad->_material.SetColor(0.25f, 0.25f, 1);

        scene.AddPrimitive( pQuad );
    }
    // Bottom Quad
    {
        Quad *pQuad = new Quad();
        pQuad->SetVertices(
            Vector<float>( -1, -1, -4),
            Vector<float>( -1, -1, 0),
            Vector<float>(  1, -1, 0) );
        pQuad->_material.SetColor(1, 1, 1);

        scene.AddPrimitive( pQuad );
    }
    // Top Quad
    {
        Quad *pQuad = new Quad();
        pQuad->SetVertices(
            Vector<float>( -1, 1, 0),
            Vector<float>( -1, 1, -4),
            Vector<float>(  1, 1, -4) );
        pQuad->_material.SetColor(1, 1, 1);

        scene.AddPrimitive( pQuad );
    }
    // Left Quad
    {
        Quad *pQuad = new Quad();
        pQuad->SetVertices(
            Vector<float>( -1,  1, 0),
            Vector<float>( -1, -1, 0),
            Vector<float>( -1, -1, -4) );
        pQuad->_material.SetColor(1, 0.25f, 0.25f);

        scene.AddPrimitive( pQuad );
    }
    // Right Quad
    {
        Quad *pQuad = new Quad();
        pQuad->SetVertices(
            Vector<float>( 1,  1, -4),
            Vector<float>( 1, -1, -4),
            Vector<float>( 1, -1,  0) );
        pQuad->_material.SetColor(0.25f, 1, 0.25f);

        scene.AddPrimitive( pQuad );
    }

    // Sphere1
    {
        Sphere* pSphere = new Sphere();
        pSphere->SetCentre( Vector<float>(0.5f, -0.75, -3) );
        pSphere->SetRadius( 0.25f );
        pSphere->_material.SetColor(1, 1, 0.25f);
        pSphere->_material.SetSpecularity( 1 );
        pSphere->_material.SetRoughness( 50 );

        scene.AddPrimitive( pSphere );
    }

    // Sphere2
    {
        Sphere* pSphere = new Sphere();
        pSphere->SetCentre( Vector<float>(-0.4f, -0.75f, -3.5f) );
        pSphere->SetRadius( 0.25f );
        pSphere->_material.SetColor(1, 1, 1);
        pSphere->_material.SetReflectivity( 1 );

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
            Vector<float>( -0.25f, 0.95f, -3),
            Vector<float>( -0.25f, 0.95f, -3.5f),
            Vector<float>(  0.25f, 0.95f, -3.5f),
            5, 5 );

        scene.AddLight( pLight );

        // Area Light Quad
        Quad *pQuad = new Quad();
        pQuad->SetVertices(
            Vector<float>( -0.25f, 0.95f, -3),
            Vector<float>( -0.25f, 0.95f, -3.5f),
            Vector<float>(  0.25f, 0.95f, -3.5f) );
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
    Serializer s( stream );
    if( !scene.Write( s ) )
    {
        std::cout << "Error: Failed to write Scene" << std::endl;
        return false;
    }

    return true;
}
