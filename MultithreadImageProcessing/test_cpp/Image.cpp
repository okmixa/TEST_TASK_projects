#include "Image.h"
#include "ImageProcessing.h"

#include "jpeg_decoder.h"

#include <fstream>
#include <iostream>
#include <algorithm>
#include <mutex>

//std::mutex g_i_mutex;

Pixel Image::getPixel(size_t x, size_t y) const
{
    return _data.at(_width * y + x);
}

void Image::setPixel(size_t x, size_t y, Pixel value)
{
 //   const std::lock_guard<std::mutex> lock(g_i_mutex);
 //   #pragma omp critical
    _data.at(_width * y + x) = value;
}

size_t Image::width()  const
{
    return _width;
}

size_t Image::height() const
{
    return _height;
}

size_t Image::area() const
{
    return width() * height();
}

Image::Image(std::string const& path)
{
    auto getFileExtension = [](std::string const& path) -> std::string {
        auto dotPos = path.rfind(".");
        std::string result = dotPos == std::string::npos ? "" : path.substr(dotPos + 1);
        std::transform(result.begin(), result.end(), result.begin(), ::tolower);
        return result;
    };
    
    std::string extension = getFileExtension(path);
    
    if (extension == "jpg" || extension == "jpeg") {
        // This mess from tiny_jpeg library example...
        FILE* f = fopen(path.c_str(), "rb");
        if (!f) {
            printf("Error opening the input file.\n");
            return;
        }
        fseek(f, 0, SEEK_END);
        size_t size = ftell(f);
        unsigned char* buf = (unsigned char*)malloc(size);
        fseek(f, 0, SEEK_SET);
        size_t read = fread(buf, 1, size, f);
        fclose(f);
        
        Jpeg::Decoder decoder(buf, size);
        if (decoder.GetResult() != Jpeg::Decoder::OK) {
            printf("Error decoding the input file\n");
            return;
        }
        
        if (decoder.IsColor()) {
            _width = decoder.GetWidth();
            _height = decoder.GetHeight();
            _data.resize(area());
            std::fill_n(_data.begin(), area(), Pixel{});
            
            for (size_t x = 0; x < width(); ++x) {
                for (size_t y = 0; y < height(); ++y) {
                    auto jpgPixel = decoder.GetImage() + (x + width() * y) * 3;
                    setPixel(x, y, Pixel{jpgPixel[0] / 255.0f, jpgPixel[1] / 255.0f, jpgPixel[2] / 255.0f});
                }
            }
        }
    } else if (extension == "ppm"){
        std::ifstream file(path, std::ifstream::binary);
        if (!file.is_open()) {
            std::cerr << "Error opening the input file \"" << path << "\"" << std::endl;
            return;
        }
        char buf[8]{};
        file.read(buf, 3);
        if (std::string(buf) == "P6\n") {
            std::fill(std::begin(buf), std::end(buf), 0);
            size_t w{}, h{};
            file >> w;
            file.ignore(1);
            file >> h;
            file.ignore(2);
            
            _width = w;
            _height = h;
            _data.resize(area());
            std::fill_n(_data.begin(), area(), Pixel{});

            for (size_t y = 0; y < height(); ++y) {
                for (size_t x = 0; x < width(); ++x) {
                    file.read(buf, 3);
                    setPixel(x, y, Pixel{buf[0] / 255.0f, buf[1] / 255.0f, buf[2] / 255.0f});
                }
            }
        }
    }
}

void Image::writeToFile(std::string const& path)
{
    auto pixel_data = std::vector<uint8_t>(width() * height() * 3);
    for (size_t y = 0; y < height(); ++y) {
        for (size_t x = 0; x < width(); ++x) {
            auto pixel = getPixel(x, y);
            size_t offset = (x + width() * y) * 3;
            pixel_data.at(offset + 0) = pixel.r * 255;
            pixel_data.at(offset + 1) = pixel.g * 255;
            pixel_data.at(offset + 2) = pixel.b * 255;
        }
    }
    
    std::ofstream file(path, std::ofstream::binary | std::ofstream::trunc);
    if (!file.is_open()) {
        std::cerr << "Error opening the output file \"" << path << "\"" << std::endl;
        return;
    }
    
    file << "P6\n"
         << static_cast<int>(width()) << " " << static_cast<int>(height()) << "\n"
         << "255\n";
    file.write(reinterpret_cast<char const*>(pixel_data.data()), pixel_data.size());
}

