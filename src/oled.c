#include "SSD1306_I2C.h"

void SSD1306_WriteCmd(SSD1306 *dev, uint8_t cmd)
{
    uint8_t x[2] = {SSD1306_CMD_REG, cmd};
    i2c_write_blocking(dev->i2cHandler, SSD1306_I2C_ADDR, x, 2, false);
}

void SSD1306_WriteData(SSD1306 *dev, uint8_t data)
{
    uint8_t x[2] = {SSD1306_DATA_REG, data};
    i2c_write_blocking(dev->i2cHandler, SSD1306_I2C_ADDR, x, 2, false);
    
    if (dev->column < 127) {
        dev->column++;
    } else {
        dev->column = 0;
    }
}

void SSD1306_SetCursor(SSD1306 *dev, uint8_t x, uint8_t y)
{
    SSD1306_WriteCmd(dev, 0x00 + (X & 0x0F)); // set column lower addres
    SSD1306_WriteCmd(dev, 0x10 + ((X >> 4) & 0x0F)); // set column higher addres
    SSD1306_WriteCmd(dev, 0xB0 + Y); // set page addres

    dev->column   = X;
    dev->page     = Y;
}

void SSD1306_NextLine(SSD1306 *dev)
{
    if (dev->page == SSD1306_PAGES-1) {
        return;
    }

    SSD1306_SetCursor(dev, 0, dev->page+1);
}

void SSD1306_ClearDisplay(SSD1306 *dev)
{
    for (uint8_t page = 0; page < 8; page++) {
        SSD1306_SetCursor(dev, 0, page);
        for (uint8_t column = 0; column < 128; column++) {
            SSD1306_WriteData(dev, 0x00);
        }
    }

    SSD1306_SetCursor(dev, 0, 0);
}

uint SSD1306_GetFontIndex(char ch, uint font_width)
{
    return (uint)((ch-0x20)*font_width);
}

void SSD1306_WriteChar(SSD1306 *dev, char ch)
{
    uint8_t font_width = dev->fontPtr->width;
    uint char_index = SSD1306_GetFontIndex(ch, font_width);

    if (dev->column + font_width >= SSD1306_WIDTH 
        && dev->page < SSD1306_PAGES-1) {
        SSD1306_SetCursor(dev, 0, dev->page+1);
        
        // If space, dont print it
        if (ch == 0x20) {
            return;
        }
    } 

    for (uint8_t i = 0; i < font_width; i++) {
        SSD1306_WriteData(dev, *((dev->fontPtr->font)+char_index+i));
    }
}

void SSD1306_WriteText(SSD1306 *dev, char *str)
{
    for (uint i = 0; i < strlen(str); i++) {
        SSD1306_WriteChar(dev, toupper(str[i]));
    }
}

void SSD1306_WriteImage(SSD1306 *dev, SSD1306_image *image, uint8_t x, uint8_t y)
{
    uint8_t img_parts = image->height/SSD1306_PAGE_HEIGHT;
    for (uint i = 0; i < img_parts; i++) {
        SSD1306_SetCursor(dev, x, i+y);
        for (uint j = 0; j < image->width; j++) {
            SSD1306_WriteData(dev, image->image[(i*image->width)+j]);
        }
    }
}

void SSD1306_SetFont(SSD1306 *dev, char *size)
{
    if (size == "sm") {
        dev->fontPtr = &font3x5;
        return;
    }

    if (size == "md") {
        dev->fontPtr = &font5x8;
        return;
    }

    if (size == "lg") {
        dev->fontPtr = &font8x8;
        return;
    }
}

void SSD1306_init(SSD1306 *dev, i2c_inst_t *i2cHandler)
{
    dev->i2cHandler = i2cHandler;

    SSD1306_WriteCmd(dev, SSD1306_SET_DISPLAY_ON);
    SSD1306_WriteCmd(dev, SSD1306_SET_DISPLAY_NORMAL);
    SSD1306_WriteCmd(dev, SSD1306_SET_MEMORY_MODE);
    SSD1306_WriteCmd(dev, SSD1306_PAGE_ADDRESSING_MODE);
    SSD1306_WriteCmd(dev, 0x8D); //charge pump
    SSD1306_WriteCmd(dev, 0x14);
    SSD1306_ClearDisplay(dev);

    SSD1306_SetFont(dev, "md");
}

int main()
{
    stdio_init_all();
    sleep_ms(2000);

    // I2C is "open drain", 
    // pull ups to keep signal high when no data is beingsent
    i2c_init(i2c_default, SSD1306_I2C_CLK * 1000);
    gpio_set_function(PICO_DEFAULT_I2C_SDA_PIN, GPIO_FUNC_I2C);
    gpio_set_function(PICO_DEFAULT_I2C_SCL_PIN, GPIO_FUNC_I2C);
    gpio_pull_up(PICO_DEFAULT_I2C_SDA_PIN);
    gpio_pull_up(PICO_DEFAULT_I2C_SCL_PIN);

    SSD1306 dev = {};

    SSD1306_init(&dev, i2c_default);

    SSD1306_WriteText(&dev, "Testowy Tekst");
    SSD1306_SetFont(&dev, "sm");
    SSD1306_NextLine(&dev);
    SSD1306_WriteText(&dev, "Testowy maly Tekst");

    SSD1306_WriteImage  (&dev, &image_heart, 0, 4);
    SSD1306_WriteImage  (&dev, &image_heart, image_heart.width + 12, 4);
    SSD1306_WriteImage  (&dev, &image_heart, image_heart.width*2 + 24, 4); 

    for (;;) {
        //
    }   

    return 0;
}