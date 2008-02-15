
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

// For detecting memory leaks
#ifdef _WIN32
    #define CRTDBG_MAP_ALLOC
    #include <stdlib.h>
    #include <crtdbg.h>
#endif

#include "Scene.h"
#include "Deserializer.h"
#include "Camera.h"
#include "Image.h"
#include "RayTracer.h"
#include "SafeDelete.h"
#include "Utility.h"
#include <iostream>
#include <fstream>
//#include <time.h>

int main(int argc, char *argv[])
{
// For detecting memory leaks
#ifdef _WIN32
    _CrtSetDbgFlag ( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );
    _CrtSetReportMode( _CRT_ERROR, _CRTDBG_MODE_DEBUG );
#endif

    //// Seed the random number generator
    //srand( (unsigned int)time( 0 ) );

    // Display license
    std::cout
        << "RayWatch - A simple cross-platform RayTracer."                  << std::endl
        << "Copyright (C) 2008"                                             << std::endl
        << "  Angelo Rohit Joseph Pulikotil,"                               << std::endl
        << "  Francis Xavier Joseph Pulikotil"                              << std::endl
        << "This program comes with ABSOLUTELY NO WARRANTY."                << std::endl
        << "This is free software, and you are welcome to redistribute it"  << std::endl
        << "under certain conditions; see <http://www.gnu.org/licenses/>."  << std::endl << std::endl;

    if( argc < 3 )
    {
        std::cout << "Insufficient arguments" << std::endl;
        std::cout << "Syntax: " << argv[0] << " <input scene filename> <output bitmap filename> [width] [height]" << std::endl;
        return -1;
    }

    // Get the required width
    int width = 500;
    if( argc > 3 )
    {
        if( !Utility::String::FromString(width, argv[3]) || (width < 1) )
        {
            std::cout << "Error: Invalid integer specified for width: " << argv[3] << std::endl;
            return -1;
        }
    }

    // Get the required height
    int height = 500;
    if( argc > 4 )
    {
        if( !Utility::String::FromString(height, argv[4]) || (height < 1) )
        {
            std::cout << "Error: Invalid integer specified for height: " << argv[4] << std::endl;
            return -1;
        }
    }

    // Create a Camera
    Camera camera;
    camera._position    .Set( 0, 0, 0 );
    camera._hFov        = 45;
    camera._vFov        = 45;

    // Create an Image
    Image image;
    if( !image.Create( width, height ) )
    {
        std::cout << "Error: Failed to create Image of size " << width << "x" << height << std::endl;
        return -1;
    }

    // Open the input scene file
    std::fstream stream;
    stream.open( argv[1], std::ios_base::in );
    if( !stream.is_open() )
    {
        std::cout << "Error: Failed to open input scene file: " << argv[1] << std::endl;
        return -1;
    }

    // Load the scene from the stream
    Scene *pScene = dynamic_cast<Scene *>( Deserializer::Read( stream ) );
    if( !pScene )
    {
        std::cout << "Error: Failed to load Scene from file: " << argv[1] << std::endl;
        return -1;
    }

    // Create a RayTracer and ray trace the scene
    RayTracer rayTracer;
    std::cout << "RayTracing";
    const bool bRTResult = rayTracer.Render( camera, *pScene, image );
    std::cout << "Done" << std::endl;

    // We're done with the scene, delete it
    SAFE_DELETE_SCALAR( pScene );

    if( !bRTResult )
    {
        std::cout << "Error: Failed while RayTracing the Scene." << std::endl;
        return -1;
    }

    // Save the image to the required output file
    if( !image.Save( argv[2] ) )
    {
        std::cout << "Error: Failed while saving image to file: " << argv[2] << std::endl;
        return -1;
    }

    return 0;
}
