
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

#include "Material.h"
#include "Ray.h"
#include "Maths.h"
#include "Scene.h"
#include "RayTracer.h"
#include "Texture.h"
#include "Deserializer.h"

// Constructor
Material::Material() :
    _color( 1 ),
    _opacity( 1 ),
    _reflectivity( 0 ),
    _fuzzyReflectionRadius( 0 ),
    _fuzzyReflectionSamples( 1 ),
    _specularity( 0 ),
    _roughness( 20 ),
    _refractiveIndex( 1 ),
    _absorption( 0 ),
    _concentration( 1 ),
    _pTexture( 0 ),
    _oneOverTextureScale( 1 ),
    _textureScale( 1 )
{
}

// Destructor
Material::~Material()
{
}

// Functions
void Material::SetColor(const float &r, const float &g, const float &b)
{
    // The _color is the amount of illumination reflected by this material;
    // hence the components must lie within the range 0 to 1.
    _color.Set(
        Maths::Bound<float>(r, 0, 1),
        Maths::Bound<float>(g, 0, 1),
        Maths::Bound<float>(b, 0, 1) );
}

void Material::SetOpacity(const float &opacity)
{
    _opacity = Maths::Bound<float>( opacity, 0, 1 );
}

void Material::SetReflectivity(const float &reflectivity)
{
    _reflectivity = Maths::Bound<float>( reflectivity, 0, 1 );
}

void Material::SetFuzzyReflectionRadius(const float &fuzzyReflectionRadius)
{
    _fuzzyReflectionRadius = Maths::Max<float>( 0, fuzzyReflectionRadius );
}

void Material::SetFuzzyReflectionSamples(const int &fuzzyReflectionSamples)
{
    _fuzzyReflectionSamples = Maths::Max<int>( 1, fuzzyReflectionSamples );
}

void Material::SetSpecularity(const float &specularity)
{
    _specularity = Maths::Bound<float>( specularity, 0, 1 );
}

void Material::SetRoughness(const float &roughness)
{
    _roughness = Maths::Max<float>( 0, roughness );
}

void Material::SetRefractiveIndex(const float &refractiveIndex)
{
    _refractiveIndex = Maths::Max<float>( 1, refractiveIndex );
}

void Material::SetAbsorption(const float &absorption)
{
    _absorption = Maths::Max<float>( 0, absorption );
}

void Material::SetConcentration(const float &concentration)
{
    _concentration = Maths::Max<float>( 0, concentration );
}

void Material::SetTexture(const Texture *const pTexture)
{
    _pTexture = pTexture;
}

void Material::SetTextureScale(const float &scale)
{
    _textureScale = scale;
    _oneOverTextureScale = 1 / scale;
}

const Color Material::GetTransmittedIllumination(
    const Ray           &incidentRay,
    const Vector<float> &surfaceNormal,
    const Scene         &scene,
    const bool          &bOnEntry,
    const float         &pathLength ) const
{
    const Vector<float> &V = incidentRay.Direction();
    const Vector<float> N = bOnEntry? surfaceNormal: -surfaceNormal;

    const float n = bOnEntry?
        1 / _refractiveIndex:   // Entering into this material
        _refractiveIndex / 1;   // Exiting from this material

    const float cosI  = -N.Dot( V );
    const float cosT2 = 1 - n * n * (1 - cosI * cosI);

    const Vector<float> T = (cosT2 > 0)?
        V * n + N * (n * cosI - sqrt(cosT2)):
        incidentRay.Direction();

    const float beersIntensity = bOnEntry? 1: exp( -_absorption * _concentration * pathLength );

    return RayTracer::GetIllumination( Ray( incidentRay.Origin(), T, incidentRay ), scene ) * beersIntensity;
}

const Color Material::GetReflectedIllumination(
    const Ray           &incidentRay,
    const Vector<float> &surfaceNormal,
    const Scene         &scene) const
{
    const Vector<float> r = incidentRay.Direction().Reflect( surfaceNormal );

    if( _fuzzyReflectionRadius > 0      &&  // If there's come fuzziness in the reflection
        incidentRay.Generation() <= 2   )   // TODO: Make this hard-coded cap on the generation configurable.
    {
        Color color( 0 );
        for(int i=0; i < _fuzzyReflectionSamples; ++i)
        {
            const Vector<float> rndVec(
                Maths::GenerateRandomValue() - 0.5f,
                Maths::GenerateRandomValue() - 0.5f,
                Maths::GenerateRandomValue() - 0.5f );

            Vector<float> rVec = rndVec - r * rndVec.Dot( r );
            rVec.Normalize();
            rVec *= _fuzzyReflectionRadius * Maths::GenerateRandomValue();

            Vector<float> fuzzyR = r + rVec;
            fuzzyR.Normalize();

            color += RayTracer::GetIllumination( Ray( incidentRay.Origin(), fuzzyR, incidentRay ), scene );
        }

        return color * (1.0f / _fuzzyReflectionSamples);
    }

    return RayTracer::GetIllumination( Ray( incidentRay.Origin(), r, incidentRay ), scene );
}

const Pixel<float> Material::GetTexel(const float &u, const float &v) const
{
    if( !_pTexture )
        return Pixel<float>( 1, 1, 1, 1 );

    return _pTexture->GetPixel( u * _oneOverTextureScale, v * _oneOverTextureScale );
}

const Color Material::GetIllumination(
    const Ray               &incidentRay,
    const Vector<float>     &surfaceNormal,
    const Scene             &scene,
    const IntersectionInfo  &intersectionInfo ) const
{
    const Pixel<float> texel = GetTexel( intersectionInfo._tU, intersectionInfo._tV );
    const Color texelColor( texel._r, texel._g, texel._b );
    const float effectiveOpacity = texel._a * _opacity;

    Color diffuse, specular;
    scene.GetSurfaceIllumination( incidentRay, surfaceNormal, _roughness, diffuse, specular );

    if( _reflectivity > 0 )
    {
        const Color reflectedIllumination = GetReflectedIllumination( incidentRay, surfaceNormal, scene );
        diffuse = Maths::InterpolateLinear(diffuse, reflectedIllumination, _reflectivity );
    }

    if( effectiveOpacity < 1 )
    {
        const Color transmittedIllumination = GetTransmittedIllumination( incidentRay, surfaceNormal, scene, intersectionInfo._bOnEntry, intersectionInfo._dist );
        diffuse = Maths::InterpolateLinear(transmittedIllumination, diffuse, effectiveOpacity );
    }

    // return the illumination not absorbed by this material
    return diffuse * _color * texelColor + specular * _specularity;
}

// Serializable's functions
const bool Material::Read(std::istream &stream)
{
    // Read the base
    if( !ReadHeader( stream, "Serializable" ) || !Serializable::Read( stream ) )
        return false;

    Color           color;
    float           opacity;
    float           reflectivity;
    float           fuzzyReflectionRadius;
    int             fuzzyReflectionSamples;
    float           specularity;
    float           roughness;
    float           refractiveIndex;
    float           absorption;
    float           concentration;
    const Texture  *pTexture = 0;
    float           textureScale;

    if( !ReadVariable( stream, "color", color )                                     ||
        !ReadVariable( stream, "opacity", opacity )                                 ||
        !ReadVariable( stream, "reflectivity", reflectivity )                       ||
        !ReadVariable( stream, "fuzzyReflectionRadius", fuzzyReflectionRadius )     ||
        !ReadVariable( stream, "fuzzyReflectionSamples", fuzzyReflectionSamples )   ||
        !ReadVariable( stream, "specularity", specularity )                         ||
        !ReadVariable( stream, "roughness", roughness )                             ||
        !ReadVariable( stream, "refractiveIndex", refractiveIndex )                 ||
        !ReadVariable( stream, "absorption", absorption )                           ||
        !ReadVariable( stream, "concentration", concentration )                     ||
        !ReadVariable( stream, "texture", pTexture )                                ||
        !ReadVariable( stream, "textureScale", textureScale )                       )
        return false;

    SetColor( color.x, color.y, color.z );
    SetOpacity( opacity );
    SetReflectivity( reflectivity );
    SetFuzzyReflectionRadius( fuzzyReflectionRadius );
    SetFuzzyReflectionSamples( fuzzyReflectionSamples );
    SetSpecularity( specularity );
    SetRoughness( roughness );
    SetRefractiveIndex( refractiveIndex );
    SetAbsorption( absorption );
    SetConcentration( concentration );
    SetTexture( pTexture );
    SetTextureScale( textureScale );

    if( !ReadFooter( stream, "Material" ) )
        return false;

    return true;
}

const bool Material::Write(std::ostream &stream) const
{
    if( !WriteHeader( stream, "Material" ) )
        return false;

    Indent();
    {
        // Write the base
        if( !Serializable::Write( stream ) )
            return false;

        if( !WriteVariable( stream, "color", _color )                                   ||
            !WriteVariable( stream, "opacity", _opacity )                               ||
            !WriteVariable( stream, "reflectivity", _reflectivity )                     ||
            !WriteVariable( stream, "fuzzyReflectionRadius", _fuzzyReflectionRadius )   ||
            !WriteVariable( stream, "fuzzyReflectionSamples", _fuzzyReflectionSamples ) ||
            !WriteVariable( stream, "specularity", _specularity )                       ||
            !WriteVariable( stream, "roughness", _roughness )                           ||
            !WriteVariable( stream, "refractiveIndex", _refractiveIndex )               ||
            !WriteVariable( stream, "absorption", _absorption )                         ||
            !WriteVariable( stream, "concentration", _concentration )                   ||
            !WriteVariable( stream, "texture", _pTexture )                              ||
            !WriteVariable( stream, "textureScale", _textureScale )                     )
            return false;
    }
    Unindent();

    if( !WriteFooter( stream, "Material" ) )
        return false;

    return true;
}

const bool Material::RestorePointers()
{
    if( !Deserializer::TranslateAddress( _pTexture ) )
        return false;

    return true;
}
