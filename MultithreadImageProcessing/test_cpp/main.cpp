#include "ImageProcessing.h"
#include "Timer.h"
#include "Test.h" /// for comparing two images (1)

#include <iostream>
#include <thread>

int main(int argc, const char * argv[]) {
    if (argc < 3) {
        std::cout << "Too few arguments. Input and output images names are required" << std::endl;
        return EXIT_FAILURE;
    }
    
    ScopeTimer overallTimer("Overall time");


    auto image = Image(argv[1]);

    {
        ScopeTimer timer("Histogram equalization time SINGLETHREAD");

           Rect roi{ 0, 0, static_cast<int>(image.width()), static_cast<int>(image.height()) };
           doHistogramEqualization(image, roi);

    }

    std::string output_file_singlethread_processing = std::string("single_thread_") + std::string(argv[2]);
    image.writeToFile(output_file_singlethread_processing.c_str());

//==========================================================================================
    Image image2 = Image(argv[1]);
    {
        ScopeTimer timer("Histogram equalization time MULTITHREAD");

        Rect roi{ 0, 0, static_cast<int>(image2.width()), static_cast<int>(image2.height()) };
        doHistogramEqualizationMultithread(image2, roi);
    }

   image2.writeToFile(argv[2]);
//==========================================================================================
    
    if (argc > 3 && argc <= 4) {
        compareTwoImages(argv[2], argv[3]); /// Compare two images (1)
    }

    return EXIT_SUCCESS;
}
