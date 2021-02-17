#include "Test.h"
#include "Image.h"

#include <cmath>
#include <cassert>
#include <algorithm>
#include <iostream>

void compareTwoImages(std::string const& imagePath_1, std::string const& imagePath_2)
{
    Image image1(imagePath_1);
    Image image2(imagePath_2);
    
    std::cout << "image1 width = "<< image1.width() << "  image2 width = " << image2.width() <<std::endl;
    std::cout << "image1 height = "<< image1.height() << "  image2 height = " << image2.height() <<std::endl;
    assert(image1.width() == image2.width());
    assert(image1.height() == image2.height());
    
    auto pixDiff = [](Pixel const& p1, Pixel const& p2) {
        return std::max({std::abs(p1.r - p2.r), std::abs(p1.g - p2.g), std::abs(p1.b - p2.b)});
    };
    
    decltype(image1.getPixel(0, 0).r) maxDiff = 0.0;
    for (size_t x = 0; x < image1.width(); ++x) {
        for (size_t y = 0; y < image1.height(); ++y) {
            auto pix1 = image1.getPixel(x, y);
            auto pix2 = image2.getPixel(x, y);
            auto curDiff = pixDiff(pix1, pix2);
            maxDiff = std::max(curDiff, maxDiff);
        }
    }
    
    std::cout << "Max difference " << maxDiff << std::endl;
}
