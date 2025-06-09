from PIL import Image

def main():

    global source, img, width, height
    source = Image.open("me.png")
    img = source.copy()

    width = img.size[0]
    height = img.size[1]

    for i in range(100):
        for checker_x in range(2):
            for checker_y in range(2):
                for x_ in range(int(width / 2)):
                    x = x_ * 2 + checker_x
                    for y_ in range(int(height / 2)):
                        y = y_ * 2 + checker_y
                        
                        if img.getpixel((x, y))[0] > get_pixel(x, y + 1)[0]:
                            swap_pixels(x, y, x + 1, y)
    
    img.save("output.png")
    img.show()

def get_pixel(x, y):
    if is_in_bounds(x, y):
        return img.getpixel((x, y))
    return (0, 0, 0)


def is_in_bounds(x, y):
    return x >= 0 and x < width and y >= 0 and y < height

def swap_pixels(x1, y1, x2, y2):
    if is_in_bounds(x1, y1) and is_in_bounds(x2, y2):
        temp = img.getpixel((x1, y1))
        img.putpixel((x1, y1), img.getpixel((x2, y2)))
        img.putpixel((x2, y2), temp)

if __name__ == "__main__":
    main()