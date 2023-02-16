#ifndef _SSD1306_FONT_
#define _SSD1306_FONT_

typedef struct{
    unsigned int    width;
    unsigned int    height;
    unsigned char   font[];
} SSD1306_font;

#endif