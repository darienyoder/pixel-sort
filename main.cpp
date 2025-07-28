#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

#include <math.h>
#include <string>
unsigned char* texture;
unsigned char* noise;
int noise_height, noise_width;

int channels;
int width, height;

bool is_in_bounds(int x, int y)
{
    return (x >= 0 && x < width && y >= 0 && y < height);
}

int coords_to_index(int x, int y)
{
    return y * width * channels + x * channels;
}

class color
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

        set_hsl();
    }

    void set_hsl() {
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
    
    color(int index) {
        load(index);
    }
    
    color(int x, int y) {
        load(coords_to_index(x, y));
    }

    color(int r_, int g_, int b_, int a_ = 255) {
        r = r_;
        g = g_;
        b = b_;
        a = a_;
    }

    float distance(color c) {
        return sqrt(pow(r - c.r, 2) + pow(g - c.g, 2) + pow(b - c.b, 2));
    }
private:
};

color get_pixel(unsigned char* image, int x, int y, int w = noise_width, int h = noise_height)
{
    int index = ((y * w + x) % (w * h)) * channels;
    return color(image[index], image[index + 1], image[index + 2], image[index + 3]);
}

void set_pixel(unsigned char* image, int x, int y, color c, int w = noise_width, int h = noise_height)
{
    int index = ((y * w + x) % (w * h)) * channels;
    image[index+0] = c.r;
    image[index+1] = c.g;
    image[index+2] = c.b;
    image[index+3] = c.a;
    //return color(image[index], image[index + 1], image[index + 2], image[index + 3]);
}

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
            // texture[index_1 + i] = texture[index_2 + i];
            texture[index_2 + i] = temp[i];
        }
    }
}

int flow_x(int x, int y)
{
    return (get_pixel(noise, x / 2 * 2, y / 2 * 2).r / 10) % 9 - 4;
}

int flow_y(int x, int y)
{
    return (get_pixel(noise, x / 2 * 2, y / 2 * 2).g / 10) % 9 - 4;
}


void process_pixel(int x, int y)
{
    color pixel(x, y);
    color compare(x + flow_x(x, y), y + flow_y(x, y));

    if (pixel.l > compare.l)
        set_pixel(texture, x + flow_x(x, y), y + flow_y(x, y), pixel);
    //swap_pixels(x, y, x + flow_x(x, y), y + flow_y(x, y));
}

int main(int argc, char const *argv[])
{
    if (argc != 2)
        return 0;
	noise = stbi_load("noise.png", &noise_width, &noise_height, &channels, 0);
	texture = stbi_load(argv[1], &width, &height, &channels, 0);

    for (int i = 0; i < 100; i++)
    {
        for (int checker_x = 0; checker_x < 1; checker_x++)
        for (int checker_y = 0; checker_y < 1; checker_y++)
        for (int x_ = 0; x_ < width / 1; x_++)
        for (int y_ = 0; y_ < height / 1; y_++)
        {
            int x = x_ * 1 + checker_x;
            int y = y_ * 1 + checker_y;

            process_pixel(x, y);
        }
        std::string path = "output/" + std::to_string(i) + ".png";
        // stbi_write_png(path.c_str(), width, height, channels, texture, width * channels);
    }

    stbi_write_png("output.png", width, height, channels, texture, width * channels);
    stbi_image_free(texture);
}