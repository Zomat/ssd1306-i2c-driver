#ifndef _SSD1306_IMAGE_
#define _SSD1306_IMAGE_

struct SSD1306_image{
    unsigned int width;
    unsigned int height;
    unsigned char image[];
};

#endif