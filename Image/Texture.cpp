
#include "Texture.h"
#include "ObjectFactory.h"

// Register with the ObjectFactory
ObjectFactory_Register(Serializable, Texture);

// Constructor
Texture::Texture() :
    _image(),
    _fileName()
{
}

// Destructor
Texture::~Texture()
{
    _image.Release();
    _fileName.clear();
}

// Functions

// Accessors
const std::string &Texture::FileName() const
{
    return _fileName;
}

const bool Texture::Load(const std::string &fileName)
{
    // Release the previous one
    _image.Release();
    _fileName.clear();

    if( !_image.SDL_Load( fileName ) )
    {
        std::cout << "Error: Failed to load image from file: " << fileName << std::endl;
        return false;
    }

    _fileName = fileName;

    return true;
}

const Pixel<float> Texture::GetPixel(const float &tu, const float &tv) const
{
    return _image.GetPixel( tu, tv );
}

// Serializable's functions

const bool Texture::Read(std::istream &stream)
{
    // Read the base
    if( !ReadHeader( stream, "Serializable" ) || !Serializable::Read( stream ) )
        return false;

    // Read the texture fileName
    {
        std::string fileName;
        if( !ReadVariable( stream, "fileName", fileName ) )
            return false;

        if( !Load( fileName ) )
            return false;
    }

    if( !ReadFooter( stream, "Texture" ) )
        return false;

    return true;
}

const bool Texture::Write(std::ostream &stream) const
{
    if( !WriteHeader( stream, "Texture" ) )
        return false;

    Indent();
    {
        // Write the base
        if( !Serializable::Write( stream ) )
            return false;

        if( !WriteVariable( stream, "fileName", _fileName ) )
            return false;
    }
    Unindent();

    if( !WriteFooter( stream, "Texture" ) )
        return false;

    return true;
}
