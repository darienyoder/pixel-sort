#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

/*
#define MOVIE_IMPLEMENTATION
#include "movie.h"
*/

#include <math.h>
#include <string>
#include <python3.12/Python.h>

unsigned char* texture;

int channels;
int width, height;

// MovieWriter* movie;

bool is_in_bounds(int x, int y)
{
    return (x >= 0 && x < width && y >= 0 && y < height);
}

int coords_to_index(int x, int y)
{
    return y * width * channels + x * channels;
}

class pixel
{
public:
    int r, g, b, a;
    float h, s, l;

    void load(int index) {
        if (index < 0 || index >= (height - 1) * width * channels + (width - 1) * channels)
            return;
        r = texture[index + 0];
        g = texture[index + 1];
        b = texture[index + 2];
        a = channels > 3 ? texture[index + 3] : 255;

        float max = std::max(r, std::max(g, b)) / 255.0;
        float min = std::min(r, std::min(g, b)) / 255.0;

        l = (max + min) / 2.0;
        s = l <= 0.5 ? (max - min) / (max + min) : (max - min) / (2.0 - max - min);
        if (max == r / 255.0)
            h = (g - b) / (max - min);
        else if (max == g / 255.0)
            h = 2.0 + (b - r) / (max - min);
        else
            h = 4.0 + (r - g) / (max - min);
        h = (h + 1) / 6.0;
    }
    
    pixel(int index) {
        load(index);
    }
    
    pixel(int x, int y) {
        load(coords_to_index(x, y));
    }
private:
};



void swap_pixels(int x1, int y1, int x2, int y2)
{
    if (is_in_bounds(x1, y1) && is_in_bounds(x2, y2))
    {
        char temp[channels];
        int index_1 = coords_to_index(x1, y1);
        int index_2 = coords_to_index(x2, y2);

        for (int i = 0; i < channels; i++)
        {
            temp[i] = texture[index_1 + i];
            texture[index_1 + i] = texture[index_2 + i];
            texture[index_2 + i] = temp[i];
        }
    }
}

int main(int argc, char const *argv[])
{
    if (argc != 2)
        return 0;
	texture = stbi_load(argv[1], &width, &height, &channels, 0);
    // movie = new MovieWriter("output/movie.mp4", width, height, 30);

    // Py_SetProgramName(argv[0]);  /* optional but recommended */
    Py_Initialize();
    PyRun_SimpleString("from time import time,ctime\n"
                        "print 'Today is',ctime(time())\n");
    Py_Finalize();

    for (int i = 0; i < 10; i++)
    {
        for (int checker_x = 0; checker_x < 2; checker_x++)
        for (int checker_y = 0; checker_y < 2; checker_y++)
        for (int x_ = 0; x_ < width / 2; x_++)
        for (int y_ = 0; y_ < height / 2; y_++)
        {
            int x = x_ * 2 + checker_x;
            int y = y_ * 2 + checker_y;

            if (pixel(x, y).l > pixel(x + 1, y).l)
                swap_pixels(x, y, x + 1, y);
            if (pixel(x, y).l > pixel(x, y + 1).h)
                swap_pixels(x, y, x, y + 1);
        }
        std::string path = "output/" + std::to_string(i) + ".png";
        // stbi_write_png(path.c_str(), width, height, channels, texture, width * channels);
        // movie->addFrame(path.c_str());
    }

    stbi_write_png("output.png", width, height, channels, texture, width * channels);
    stbi_image_free(texture);
}