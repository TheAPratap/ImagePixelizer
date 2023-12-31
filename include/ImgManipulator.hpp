#ifndef MAIN_HPP
#define MAIN_HPP

#include <iostream>
#include <fstream>
#include <sstream>
#include <cmath>
using namespace std;


class ImageEdits
{
    private:
        int width = 0;
        int height = 0;
        int color = 0;
        string type = "P3";

    public:
        ~ImageEdits();
        ImageEdits() {}
        struct RGB 
        {
            unsigned char red;
            unsigned char green;
            unsigned char blue;
        };

        void set_width(int width)
        {
            this->width = width;
        }
        
        void set_height(int height)
        {
            this->height = height;
        }

        void read_image(string file_name);
        void write_image(string file_name);
        void delete_image();
        void horizontal_flip();
        void vertical_flip();
        void resize(int height, int weight);
        void applyfilter_red();
        void applyfilter_green();
        void applyfilter_blue();
        void applyfilter_grayscale();
        void adjust_brightness(int amount);
        void apply_blur(int amount);
        void floydDithered();
        void edgeDetection();
        void quantize2bit();
        void raise_warning_message();
        RGB **image = nullptr;
};

#endif
