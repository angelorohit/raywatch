
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

void Examples::Example2(const std::string &fileName)
{
    // Create a Scene
    Scene scene;
    scene._maxRayGenerations = 6;
    scene._ambientLight.Set( 0.1f );

    // Create textures
    Image texture1;
    texture1.SDL_Load( "Textures/Checks.jpg" );

    Image texture2;
    texture2.SDL_Load( "Textures/Strands.png" );

    // Bottom Quad
    {
        Quad *pQuad = new Quad();
        pQuad->SetVertices(
            Vector<float>( -2, -1, -6),
            Vector<float>( -2, -1, -2),
            Vector<float>(  2, -1, -2) );
        pQuad->_material.SetColor(1, 1, 1);
        pQuad->_material.SetTexture( &texture1 );
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

        pSphere->_material.SetTexture( &texture2 );

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

        pSphere->_material.SetTexture( &texture2 );

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

    // Create a Camera
    Camera camera;
    camera._position    .Set( 0, 0, 0 );
    camera._hFov        = 45;
    camera._vFov        = 45;

    // Create an Image
    Image image;
    image.Create( 500, 500 );

    // Create a RayTracer and ray trace the scene
    RayTracer rayTracer;
    rayTracer.Render( camera, scene, image );

    // Save the image to a file for inspection.
    image.Save( fileName );
}
