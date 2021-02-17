#include "ImageProcessing.h"

#include <array>
#include <cassert>
#include <thread>
#include "Timer.h"

enum RGB {R = 0, G = 1, B = 2};
constexpr auto MAX_VALUE = std::numeric_limits<uint16_t>::max();
using Histogram = std::array<size_t, MAX_VALUE>;
const  auto MAX_THREADS = std::thread::hardware_concurrency();

void calculateHistogram(Image image, Rect roi, int channel, Histogram& histogram)
{
    ScopeTimer timer(" calculate Histogram  time");

    for (size_t x = roi.x; x < roi.width; ++x) {
        for (size_t y = roi.y; y < roi.height; ++y) {
            auto pixel = image.getPixel(x, y);
            histogram[(*(reinterpret_cast<double*>(&pixel) + channel) * MAX_VALUE)]++;
        }
    }
}

void calculateCFH(const Histogram& histogram, Histogram& cfh)
{

    size_t sum = 0;
    for(size_t i = 0; i < histogram.size(); ++i) {
        cfh[i] = sum += histogram[i];
    }
}

void equalizeChannel(Image& image, Rect roi, int channel, Histogram const cfh)
{
    ScopeTimer timer(" equalize Channel time ");

    for (size_t x = roi.x; x < roi.x + roi.width; ++x) {
        for (size_t y = roi.y; y < roi.y + roi.height; ++y) {
            auto pixel = image.getPixel(x, y);
            auto currentChannel = (reinterpret_cast<double*>(&pixel) + channel);
            *currentChannel = cfh[*currentChannel * (MAX_VALUE - 1)] * static_cast<double>(MAX_VALUE) / image.area() / MAX_VALUE;
            image.setPixel(x, y, pixel);
        }
    }
}

void doHistogramEqualization(Image& image, Rect roi)
{
    // Image include ROI ?
    assert(roi.x >= 0);
    assert(roi.x + roi.width <= image.width());
    assert(roi.y >= 0);
    assert(roi.y + roi.height <= image.height());
    

    for (int ch = 0; ch < 3; ++ch) {


        Histogram hist{}, cfh{};

        calculateHistogram(image, roi, ch, hist);
        calculateCFH(hist, cfh);
        equalizeChannel(image, roi, ch, cfh);

    }


}

void doHistogramEqualizationMultithread(Image& image, Rect roi)
{
    // Image include ROI ?
    assert(roi.x >= 0);
    assert(roi.x + roi.width <= image.width());
    assert(roi.y >= 0);
    assert(roi.y + roi.height <= image.height());


   //Once calculate channels in histograms
   std::thread ttCalculateHistogram[3];
   Histogram hist[3], cfh[3];

   for (int ch = 0; ch < 3; ++ch) {
      ttCalculateHistogram[ch] = std::thread([&image, ch, &roi, &hist, &cfh]() {

         calculateHistogram(image, roi, ch, hist[ch]);
         calculateCFH(hist[ch], cfh[ch]);

       });
    }

    for (int ch = 0; ch < 3; ++ch)
      ttCalculateHistogram[ch].join();


    std::thread *ttEqualize = new std::thread[MAX_THREADS];

    for (int tt = 0; tt < MAX_THREADS; ++tt) {

        ttEqualize[tt] = std::thread([&image, &cfh, tt]() {

            //cut entrie Image to several separate revtangles, start equalizer for each in separate thread
            int rectangleWidth = static_cast<int>(image.width()) / MAX_THREADS ;
            Rect roi{  rectangleWidth * tt , 0, tt < (MAX_THREADS - 1) ? rectangleWidth : static_cast<int>(image.width()) - rectangleWidth * tt, static_cast<int>(image.height()) };

            //add channels
            for(int ch =0; ch < 3; ++ch)
                equalizeChannel(image, roi, ch, cfh[ch]);

        });
    }

   for (int tt = 0; tt < MAX_THREADS; ++tt)
      ttEqualize[tt].join();

}
