
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

#ifndef MATERIAL_HEADER
#define MATERIAL_HEADER

#include "Color.h"
#include "IntersectionInfo.h"
#include "Pixel.h"

// Forward Declarations
class Ray;
class Scene;
class Image;

class Material
{
// Members
private:
    Color   _color; // The amount of illumination reflected by this material;
                    // components must be within the range 0 to 1.

    float   _opacity;                   // Value ranges from 0 to 1; 0 being fully transparent and 1 being fully opaque.
    float   _reflectivity;              // Value ranges from 0 to 1; 0 being not reflective at all and 1 being fully reflective.
    float   _fuzzyReflectivity;         // Fuzzy reflection radius; 0 for perfect (non-fuzzy) reflections and >0 for increasingly fuzzy reflections
    int     _fuzzyReflectionSamples;    // Number of samples to take for fuzzy reflections
    float   _specularity;               // Value ranges from 0 to 1; 0 being not specular at all and 1 having full specular.
    float   _roughness;                 // Smaller values give larger specular highlights, larger values give smaller sharper specular highlights
    float   _refractiveIndex;
    float   _absorption;                // The amount of light absorbed by the material
    float   _concentration;             // The concentration of the material; density of the material.

    const Image *_pTexture;             // Texture to apply on the material
    float        _oneOverTextureScale;  // 1 / scale to be applied to the texture

public:
// Constructor
    explicit Material();
// Destructor
    ~Material();

private:
// Copy Constructor / Assignment Operator
    Material(const Material &);
    const Material &operator =(const Material &);

// Functions
private:
    const Color GetTransmittedIllumination(
        const Ray           &incidentRay,
        const Vector<float> &surfaceNormal,
        const Scene         &scene,
        const bool          &bOnEntry,
        const float         &pathLength ) const;

    const Color GetReflectedIllumination(
        const Ray           &incidentRay,
        const Vector<float> &surfaceNormal,
        const Scene         &scene) const;

    const Pixel<float> GetTexel(const float &u, const float &v) const;

public:
    void SetColor(const float &r, const float &g, const float &b);
    void SetOpacity(const float &opacity);
    void SetReflectivity(const float &reflectivity);
    void SetFuzzyReflectivity(const float &fuzzyReflectivity);
    void SetFuzzyReflectionSamples(const int &fuzzyReflectionSamples);
    void SetSpecularity(const float &specularity);
    void SetRoughness(const float &roughness);
    void SetRefractiveIndex(const float &refractiveIndex);
    void SetAbsorption(const float &absorption);
    void SetConcentration(const float &concentration);

    void SetTexture(const Image *const pTexture);
    void SetTextureScale(const float &scale);

    const Color GetIllumination(
        const Ray               &incidentRay,
        const Vector<float>     &surfaceNormal,
        const Scene             &scene,
        const IntersectionInfo  &intersectionInfo ) const;
};

#endif
