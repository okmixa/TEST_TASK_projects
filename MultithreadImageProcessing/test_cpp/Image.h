#pragma once

#include <vector>
#include <string>

struct Pixel
{
    double r = 0.0f, g = 0.0f, b = 0.0f;
};

class Image
{
public:
    Image(std::string const& path); // Support only jpeg format with baseline encoding and ppm p6 binary format and jpeg
    Image(size_t width, size_t height) : _width(width),
                                         _height(height),
                                         _data(width * height)
    {}
    Pixel getPixel(size_t x, size_t y) const;
    void setPixel(size_t x, size_t y, Pixel value);
    size_t width()  const;
    size_t height() const;
    size_t area() const;
    
    void writeToFile(std::string const& path); // Support only ppm p6 binary format
    
private:
    size_t _width = 0, _height = 0;
    std::vector<Pixel> _data;
};
