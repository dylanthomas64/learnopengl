#ifndef PIXELSORT_H
#define PIXELSORT_H

#include <vector>
#include <algorithm>
#include <iostream>


// RGB to HSL colour space conversation taken from stack overflow: https://stackoverflow.com/a/6930407/15672951
typedef struct {
    double r;       // a fraction between 0 and 1
    double g;       // a fraction between 0 and 1
    double b;       // a fraction between 0 and 1
} rgb;

typedef struct {
    double h;       // angle in degrees
    double s;       // a fraction between 0 and 1
    double v;       // a fraction between 0 and 1
} hsv;

static hsv   rgb2hsv(rgb in);
static rgb   hsv2rgb(hsv in);

hsv rgb2hsv(rgb in)
{
    hsv         out;
    double      min, max, delta;

    min = in.r < in.g ? in.r : in.g;
    min = min < in.b ? min : in.b;

    max = in.r > in.g ? in.r : in.g;
    max = max > in.b ? max : in.b;

    out.v = max;                                // v
    delta = max - min;
    if (delta < 0.00001)
    {
        out.s = 0;
        out.h = 0; // undefined, maybe nan?
        return out;
    }
    if (max > 0.0) { // NOTE: if Max is == 0, this divide would cause a crash
        out.s = (delta / max);                  // s
    }
    else {
        // if max is 0, then r = g = b = 0              
        // s = 0, h is undefined
        out.s = 0.0;
        out.h = NAN;                            // its now undefined
        return out;
    }
    if (in.r >= max)                           // > is bogus, just keeps compilor happy
        out.h = (in.g - in.b) / delta;        // between yellow & magenta
    else
        if (in.g >= max)
            out.h = 2.0 + (in.b - in.r) / delta;  // between cyan & yellow
        else
            out.h = 4.0 + (in.r - in.g) / delta;  // between magenta & cyan

    out.h *= 60.0;                              // degrees

    if (out.h < 0.0)
        out.h += 360.0;

    return out;
}


rgb hsv2rgb(hsv in)
{
    double      hh, p, q, t, ff;
    long        i;
    rgb         out;

    if (in.s <= 0.0) {       // < is bogus, just shuts up warnings
        out.r = in.v;
        out.g = in.v;
        out.b = in.v;
        return out;
    }
    hh = in.h;
    if (hh >= 360.0) hh = 0.0;
    hh /= 60.0;
    i = (long)hh;
    ff = hh - i;
    p = in.v * (1.0 - in.s);
    q = in.v * (1.0 - (in.s * ff));
    t = in.v * (1.0 - (in.s * (1.0 - ff)));

    switch (i) {
    case 0:
        out.r = in.v;
        out.g = t;
        out.b = p;
        break;
    case 1:
        out.r = q;
        out.g = in.v;
        out.b = p;
        break;
    case 2:
        out.r = p;
        out.g = in.v;
        out.b = t;
        break;

    case 3:
        out.r = p;
        out.g = q;
        out.b = in.v;
        break;
    case 4:
        out.r = t;
        out.g = p;
        out.b = in.v;
        break;
    case 5:
    default:
        out.r = in.v;
        out.g = p;
        out.b = q;
        break;
    }
    return out;
}

struct Pixel {
    unsigned char R = 0;
    unsigned char G = 0;
    unsigned char B = 0;
    unsigned int H = 0;//hue angle in degrees
    unsigned char S = 0;//saturation
    unsigned char L = 0;//lightness
};



// only works for RGB (3 channels)
std::vector<unsigned char> pixelsort(const unsigned char* data, int width, int height, int nrChannels, const unsigned int LIGHT_THRESHOLD_LOWER, const unsigned int LIGHT_THRESHOLD_UPPER) {
    // PIXEL SORTTT!
    std::cout << "Sorting pixels...\n";
    size_t size = width * height * nrChannels;

    
    


    // create 2d vector for height number of rows of width
    // height x width = vec[y][x]

    std::vector<std::vector<Pixel>> pixel_data_vec{};
    std::vector<Pixel> pixel_row{};
    Pixel pixel;
    rgb pixel_rgb;
    hsv pixel_hsv;
    for (int h = 0; h < height; h++) {
        for (int x = 0; x < (width * 3); x += 3) {
            pixel.R = data[(h * width * 3) + x];
            pixel.G = data[(h * width * 3) + x + 1];
            pixel.B = data[(h * width * 3) + x + 2];
            //calculate hsv values
            pixel_rgb.r = pixel.R/255.0;
            pixel_rgb.g = pixel.G/255.0;
            pixel_rgb.b = pixel.B/255.0;
            pixel_hsv = rgb2hsv(pixel_rgb);
            pixel.H = pixel_hsv.h;
            pixel.S = pixel_hsv.s*255.0;
            pixel.L = pixel_hsv.v*255.0;

            pixel_row.push_back(pixel);
        }
        pixel_data_vec.push_back(pixel_row);
        pixel_row.clear();
    }

   


    // actual PIXEL SORTING here


    //initialise masks
    //std::vector<std::vector<unsigned int>> mask_vec{};
    std::vector<unsigned int> mask_row{};
    std::vector<std::vector<Pixel>> sorted_pixel_data_vec{};
    std::vector<Pixel> sorted_row{};


    for (std::vector<Pixel>& row : pixel_data_vec) {
        // sort by lightness
        //std::sort(row.begin(), row.end(), [](Pixel& lhs, Pixel& rhs) {return lhs.L < rhs.L; });

        
        for (Pixel& pixel : row) {
            mask_row.push_back((pixel.L < LIGHT_THRESHOLD_LOWER || pixel.L > LIGHT_THRESHOLD_UPPER) ? 0 : 1);
        }

        //std::cout << "row: " << row.size() << "\nmask: " << mask_row.size() << std::endl;
        // iterate through row and mask at same time. when a mask value is 0 stop and sort current range
        std::vector<Pixel> sub_range{};
        for (size_t i = 0; i < row.size(); i++) {

            if (mask_row.at(i) == 0)
            {
                if (!sub_range.empty()) {
                    // reached a 0, so sort sub_row in place
                    std::sort(sub_range.begin(), sub_range.end(), [](Pixel& lhs, Pixel& rhs) {return lhs.L < rhs.L; });
                    // and insert sorted vec to end of sorted_row vec
                    sorted_row.insert(sorted_row.end(), sub_range.begin(), sub_range.end());
                    sub_range.clear();
                }
                

                // push back original pixel
                sorted_row.push_back(row.at(i));
               // std::cout << i << ", ";
            }
            else if (mask_row.at(i) == 1)
            {
                sub_range.push_back(row.at(i));
                //std::cout << i << ", ";
            }
            else {
                std::cout << "ermmmmm? not good\n" << std::endl;
            }
           

        }
        //row finished so make sure to add remaining sub_range before next row in loop
        if (!sub_range.empty()) {
            // reached a 0, so sort sub_row in place
            std::sort(sub_range.begin(), sub_range.end(), [](Pixel& lhs, Pixel& rhs) {return lhs.L < rhs.L; });
            // and insert sorted vec to end of sorted_row vec
            sorted_row.insert(sorted_row.end(), sub_range.begin(), sub_range.end());
            sub_range.clear();
        }

        sorted_pixel_data_vec.push_back(sorted_row);
        sorted_row.clear();
        mask_row.clear();
    }

    std::cout << "sorting completed\n";
    





    
    long light_total = 0;
    //maps pixel_data_vec onto a raw vector of chars from which to eventually call .data()
    std::vector<unsigned char> raw_pixel_vec{};
   
    for (std::vector<Pixel> row : sorted_pixel_data_vec) {
        for (Pixel pixel : row) {
            raw_pixel_vec.push_back(pixel.R);
            raw_pixel_vec.push_back(pixel.G);
            raw_pixel_vec.push_back(pixel.B);
            // get light value to calculate average level
            light_total += pixel.L;
        }
    }

    std::cout << "average light [0-255] : " << light_total / raw_pixel_vec.size() << std::endl;


    return raw_pixel_vec;
}





#endif // !PIXELSORT_H
