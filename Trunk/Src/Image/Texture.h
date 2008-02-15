
#ifndef TEXTURE_HEADER
#define TEXTURE_HEADER

#include "Image.h"
#include "Serializable.h"
#include <string>

class Texture : public Serializable
{
// Members
private:
    Image           _image;
    std::string     _fileName;

public:
// Constructor
    explicit Texture();
// Destructor
    virtual ~Texture();

private:
// Copy Constructor / Assignment Operator
    Texture(const Texture &);
    const Texture &operator =(const Texture &);

// Functions
public:
    // Accessors
    const std::string &FileName() const;

    const bool Load(const std::string &fileName);
    const Pixel<float> GetPixel(const float &tu, const float &tv) const;

    // Serializable's functions
    virtual const bool Read(std::istream &stream);
    virtual const bool Write(std::ostream &stream) const;
};

#endif
