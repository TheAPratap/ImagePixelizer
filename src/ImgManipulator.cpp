#include<iostream>
#include<fstream>
#include<sstream>
#include "../include/ImgManipulator.hpp"
using namespace std;

ImageEdits::~ImageEdits()
{
    delete_image();
}

void ImageEdits::read_image(string file_name)
{
    ifstream input(file_name, ios::binary);
    if (input)
    {
        input >> type >> width >> height >> color;
        if (type == "P3")
        {
            image = new RGB*[height];
            for (int i=0; i<height; i++)
            {
                image[i] = new RGB[width];
                for (int j=0; j<width; j++)
                {
                    int r, g, b;
                    input >> r >> g >> b;
                    image[i][j].red = r;
                    image[i][j].green = g;
                    image[i][j].blue = b;
                }
            }
            input.close();
        }
    }
    else
        raise_warning_message();
}

void ImageEdits::write_image(string file_name)
{
    if (image != nullptr)
    {
        ofstream output(file_name, ios::binary);
        if (output.is_open())
        {
            output << type << endl;
            output << width << " " << height << endl;
            output << 255 << endl;

            if (type == "P3")
            {
                for (int i=0; i<height; i++)
                {
                    for (int j=0; j<width; j++)
                    {
                        output << (int) image[i][j].red << " ";
                        output << (int) image[i][j].green << " ";
                        output << (int) image[i][j].blue << " ";
                    }
                }
            }
            output.close();
        }
    }
    else
        raise_warning_message();
}

void ImageEdits::horizontal_flip()
{
    if (image != nullptr)
    {
        for (int i=0; i<height; i++)
            for(int j=0; j<width/2; j++)
                swap(image[i][j], image[i][width-1-j]);
    }
    else
        raise_warning_message();
}

void ImageEdits::vertical_flip()
{
    if (image != nullptr)
    {
        for (int i=0; i<height/2; i++)
            for (int j=0; j<width; j++) 
                swap(image[i][j], image[height-1-i][j]);
    }
    else
        raise_warning_message();
}

void ImageEdits::resize(int height, int width)
{
    if (image != nullptr)
    {
        RGB **image_resized = new RGB*[height];
        for (int i=0; i<height; i++)
        {
            image_resized[i] = new RGB[width];
            for (int j=0; j<width; j++)
            {
                image_resized[i][j].red = 255;
                image_resized[i][j].green = 255;
                image_resized[i][j].blue = 255;
            }
        }

        for (int i = 0; i < height; i++) 
            for (int j=0; j<width; j++)
                image_resized[i][j] = image[i*this->height/height][j*this->width/width];
        delete_image();
        image = image_resized;
        this -> height = height;
        this -> width = width;
    }
    else
        raise_warning_message();
}

void ImageEdits::delete_image()
{
    if(image != nullptr)
    {
        for (int i=0; i< height; i++)
            delete image[i];

        delete image;
    }
}

void ImageEdits::applyfilter_red()
{
    if (image != nullptr)
    {
        for (int i=0; i<height; i++) 
            for (int j=0; j<width; j++)
                image[i][j].red = (image[i][j].red+50)>255 ? 255 : image[i][j].red+=50;
    }  
    else
        raise_warning_message();
}

void ImageEdits::applyfilter_green()
{
    if (image != nullptr)
    {
        for (int i=0; i<height; i++)
            for (int j=0; j<width; j++)
                image[i][j].green = (image[i][j].green+50)>255 ? 255 : image[i][j].green+=50;
    }
    else
        raise_warning_message();
}

void ImageEdits::applyfilter_blue()
{
    if (image != nullptr)
    {
        for (int i=0; i<height; i++) 
            for (int j=0; j<width; j++)
                image[i][j].blue = (image[i][j].blue+50)>255 ? 255 : image[i][j].blue+=50;
    } 
    else
        raise_warning_message();
}

void ImageEdits::applyfilter_grayscale()
{
    if (image != nullptr)
    {
        const float r = 0.299f;
        const float g = 0.587f;
        const float b = 0.114f;
        float grayscaleValue;

        for (int i=0; i<height; i++)
        {
            for (int j=0; j<width; j++)
            {
                grayscaleValue = image[i][j].red*r + image[i][j].green*g + image[i][j].blue*b;
                image[i][j].red = grayscaleValue;
                image[i][j].green = grayscaleValue;
                image[i][j].blue = grayscaleValue;
            }
        }
    }
    else
        raise_warning_message();
}

void ImageEdits::adjust_brightness(int amount)
{
    if (image != nullptr)
    {
        if (amount > 0)
        {
            for (int i=0; i<height; i++)
            {
                for (int j=0; j<width; j++)
                {
                    image[i][j].red = (image[i][j].red+image[i][j].red*(amount/100))<255 ? image[i][j].red+image[i][j].red*(amount/100) : 255;
                    image[i][j].green = (image[i][j].green+image[i][j].green*(amount/100))<255 ? image[i][j].green+image[i][j].green*(amount/100) : 255;
                    image[i][j].blue = (image[i][j].blue+image[i][j].blue*(amount/100))<255 ? image[i][j].blue+image[i][j].blue*(amount/100) : 255;
                }
            }   
        }
        else 
        {
            for (int i=0; i<height; i++)
            {
                for (int j=0; j<width; j++)
                {
                    image[i][j].red = (image[i][j].red+amount)>0 ? image[i][j].red+amount : 0;
                    image[i][j].green = (image[i][j].green+amount)>0 ? image[i][j].green+amount : 0;
                    image[i][j].blue = (image[i][j].blue+amount)>0 ? image[i][j].blue+amount : 0;

                }
            }   
        }
    }
    else
        raise_warning_message();
}

void ImageEdits::apply_blur(int amount)
{
    if (image != nullptr && type == "P3")
    {
        // Create a temporary image to store the blurred result
        RGB** blurred_image = new RGB*[height];
        for (int i = 0; i < height; i++)
        {
            blurred_image[i] = new RGB[width];
        }

        // Calculate the blur radius
        int blur_radius = amount;

        // Create an integral image to compute sums efficiently
        int** integral_red = new int*[height];
        int** integral_green = new int*[height];
        int** integral_blue = new int*[height];

        for (int i = 0; i < height; i++)
        {
            integral_red[i] = new int[width]();
            integral_green[i] = new int[width]();
            integral_blue[i] = new int[width]();
        }

        // Initialize the integral image with the first row and column
        for (int i = 0; i < height; i++)
        {
            for (int j = 0; j < width; j++)
            {
                integral_red[i][j] = image[i][j].red;
                integral_green[i][j] = image[i][j].green;
                integral_blue[i][j] = image[i][j].blue;

                if (i > 0)
                {
                    integral_red[i][j] += integral_red[i - 1][j];
                    integral_green[i][j] += integral_green[i - 1][j];
                    integral_blue[i][j] += integral_blue[i - 1][j];
                }

                if (j > 0)
                {
                    integral_red[i][j] += integral_red[i][j - 1];
                    integral_green[i][j] += integral_green[i][j - 1];
                    integral_blue[i][j] += integral_blue[i][j - 1];
                }

                if (i > 0 && j > 0)
                {
                    integral_red[i][j] -= integral_red[i - 1][j - 1];
                    integral_green[i][j] -= integral_green[i - 1][j - 1];
                    integral_blue[i][j] -= integral_blue[i - 1][j - 1];
                }
            }
        }

        // Apply blur to each pixel using the integral image
        for (int i = 0; i < height; i++)
        {
            for (int j = 0; j < width; j++)
            {
                int x1 = max(0, j - blur_radius);
                int y1 = max(0, i - blur_radius);
                int x2 = min(width - 1, j + blur_radius);
                int y2 = min(height - 1, i + blur_radius);

                int total_red = integral_red[y2][x2];
                int total_green = integral_green[y2][x2];
                int total_blue = integral_blue[y2][x2];

                if (x1 > 0)
                {
                    total_red -= integral_red[y2][x1 - 1];
                    total_green -= integral_green[y2][x1 - 1];
                    total_blue -= integral_blue[y2][x1 - 1];
                }

                if (y1 > 0)
                {
                    total_red -= integral_red[y1 - 1][x2];
                    total_green -= integral_green[y1 - 1][x2];
                    total_blue -= integral_blue[y1 - 1][x2];
                }

                if (x1 > 0 && y1 > 0)
                {
                    total_red += integral_red[y1 - 1][x1 - 1];
                    total_green += integral_green[y1 - 1][x1 - 1];
                    total_blue += integral_blue[y1 - 1][x1 - 1];
                }

                int neighborhood_size = (x2 - x1 + 1) * (y2 - y1 + 1);
                blurred_image[i][j].red = total_red / neighborhood_size;
                blurred_image[i][j].green = total_green / neighborhood_size;
                blurred_image[i][j].blue = total_blue / neighborhood_size;
            }
        }

        // Clean up the integral image
        for (int i = 0; i < height; i++)
        {
            delete[] integral_red[i];
            delete[] integral_green[i];
            delete[] integral_blue[i];
        }
        delete[] integral_red;
        delete[] integral_green;
        delete[] integral_blue;

        // Replace the original image with the blurred image
        delete_image();
        image = blurred_image;
    }
    else
    {
        raise_warning_message();
    }
}

void ImageEdits::floydDithered()
{
    if (image != nullptr && type == "P3")
    {
        // Create a temporary image to store the dithered result
        RGB** dithered_image = new RGB*[height];
        for (int i = 0; i < height; i++)
        {
            dithered_image[i] = new RGB[width];
        }

        // Apply Floyd-Steinberg dithering
        for (int i = 0; i < height; i++)
        {
            for (int j = 0; j < width; j++)
            {
                // Current pixel
                RGB& current_pixel = image[i][j];

                // Old pixel values
                int old_red = current_pixel.red;
                int old_green = current_pixel.green;
                int old_blue = current_pixel.blue;

                // New pixel values after dithering (binary thresholding)
                int new_red = (old_red < 128) ? 0 : 255;
                int new_green = (old_green < 128) ? 0 : 255;
                int new_blue = (old_blue < 128) ? 0 : 255;

                // Error diffusion
                int error_red = old_red - new_red;
                int error_green = old_green - new_green;
                int error_blue = old_blue - new_blue;

                // Update the current pixel
                current_pixel.red = new_red;
                current_pixel.green = new_green;
                current_pixel.blue = new_blue;

                // Diffuse the error to neighboring pixels
                if (j < width - 1)
                {
                    image[i][j + 1].red += (error_red * 7) / 16;
                    image[i][j + 1].green += (error_green * 7) / 16;
                    image[i][j + 1].blue += (error_blue * 7) / 16;
                }

                if (i < height - 1)
                {
                    if (j > 0)
                    {
                        image[i + 1][j - 1].red += (error_red * 3) / 16;
                        image[i + 1][j - 1].green += (error_green * 3) / 16;
                        image[i + 1][j - 1].blue += (error_blue * 3) / 16;
                    }

                    image[i + 1][j].red += (error_red * 5) / 16;
                    image[i + 1][j].green += (error_green * 5) / 16;
                    image[i + 1][j].blue += (error_blue * 5) / 16;

                    if (j < width - 1)
                    {
                        image[i + 1][j + 1].red += (error_red * 1) / 16;
                        image[i + 1][j + 1].green += (error_green * 1) / 16;
                        image[i + 1][j + 1].blue += (error_blue * 1) / 16;
                    }
                }
            }
        }

        // Copy the dithered image to the result
        for (int i = 0; i < height; i++)
        {
            for (int j = 0; j < width; j++)
            {
                dithered_image[i][j] = image[i][j];
            }
        }

        // Replace the original image with the dithered image
        delete_image();
        image = dithered_image;
    }
    else
    {
        raise_warning_message();
    }
}

void ImageEdits::edgeDetection()
{
    if (image != nullptr && type == "P3")
    {
        // Define Sobel kernels for horizontal and vertical edges
        const int sobel_horizontal_kernel[3][3] = {
            { -1, 0, 1 },
            { -2, 0, 2 },
            { -1, 0, 1 }
        };

        const int sobel_vertical_kernel[3][3] = {
            { -1, -2, -1 },
            { 0, 0, 0 },
            { 1, 2, 1 }
        };

        // Apply Sobel edge detection directly to the original image
        for (int i = 1; i < height - 1; i++)
        {
            for (int j = 1; j < width - 1; j++)
            {
                int gx_red = 0, gx_green = 0, gx_blue = 0;
                int gy_red = 0, gy_green = 0, gy_blue = 0;

                // Convolve the Sobel kernels with the neighborhood
                for (int k = -1; k <= 1; k++)
                {
                    for (int l = -1; l <= 1; l++)
                    {
                        int ni = i + k;
                        int nj = j + l;
                        int kernel_value_h = sobel_horizontal_kernel[k + 1][l + 1];
                        int kernel_value_v = sobel_vertical_kernel[k + 1][l + 1];

                        gx_red += image[ni][nj].red * kernel_value_h;
                        gx_green += image[ni][nj].green * kernel_value_h;
                        gx_blue += image[ni][nj].blue * kernel_value_h;

                        gy_red += image[ni][nj].red * kernel_value_v;
                        gy_green += image[ni][nj].green * kernel_value_v;
                        gy_blue += image[ni][nj].blue * kernel_value_v;
                    }
                }

                // Calculate the magnitude of the gradient
                int magnitude_red = sqrt(gx_red * gx_red + gy_red * gy_red);
                int magnitude_green = sqrt(gx_green * gx_green + gy_green * gy_green);
                int magnitude_blue = sqrt(gx_blue * gx_blue + gy_blue * gy_blue);

                // Ensure that the magnitude is within the valid range [0, 255]
                magnitude_red = min(255, max(0, magnitude_red));
                magnitude_green = min(255, max(0, magnitude_green));
                magnitude_blue = min(255, max(0, magnitude_blue));

                // Set the pixel values directly in the original image
                image[i][j].red = magnitude_red;
                image[i][j].green = magnitude_green;
                image[i][j].blue = magnitude_blue;
            }
        }
    }
    else
    {
        raise_warning_message();
    }
}

void ImageEdits::quantize2bit()
{
    if (image != nullptr && type == "P3")
    {
        const unsigned char mask = 0xC0; // Mask to keep the 2 most significant bits (11000000)

        // Apply 2-bit quantization to each color channel (red, green, blue)
        for (int i = 0; i < height; i++)
        {
            for (int j = 0; j < width; j++)
            {
                // Quantize the red channel
                image[i][j].red = (image[i][j].red & mask);

                // Quantize the green channel
                image[i][j].green = (image[i][j].green & mask);

                // Quantize the blue channel
                image[i][j].blue = (image[i][j].blue & mask);
            }
        }
    }
    else
    {
        raise_warning_message();
    }
}


void ImageEdits::raise_warning_message()
{
    cout << "Image not read properly \n";
}
