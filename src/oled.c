#include <stdio.h>
#include <string.h>
#include <pico/stdlib.h>
#include <ctype.h>
#include "pico/stdlib.h"
#include "pico/binary_info.h"
#include "hardware/i2c.h"
#include "ssd1306_font_5x8.h"
#include "ssd1306_font_8x8.h"
#include "ssd1306_image.h"
#include "image_raspberry.h"
#include "image_heart.h"
#include "image_maggie.h"

#define SSD1306_HEIGHT                  64
#define SSD1306_WIDTH                   128

#define SSD1306_I2C_ADDR                _u(0x3C)
#define SSD1306_I2C_CLK                 400

#define SSD1306_SET_DISPLAY_ON          _u(0xAF)
#define SSD1306_SET_DISPLAY_OFF         _u(0xAF)
#define SSD1306_SET_DISPLAY_NORMAL      _u(0xA6)
#define SSD1306_SET_MEMORY_MODE         _u(0x20)
#define SSD1306_PAGE_ADDRESSING_MODE    _u(0x02)

#define SSD1306_CMD_REG                 _u(0x80)
#define SSD1306_DATA_REG                _u(0x40)

uint8_t (*font) = font8x8;

void SSD1306_write_cmd(uint8_t cmd)
{
    uint8_t x[2] = {SSD1306_CMD_REG, cmd};
    i2c_write_blocking(i2c_default, SSD1306_I2C_ADDR, x, count_of(x), false);
}

void SSD1306_write_data(uint8_t data)
{
    uint8_t x[2] = {SSD1306_DATA_REG, data};
    i2c_write_blocking(i2c_default, SSD1306_I2C_ADDR, x, count_of(x), false);
}

void SSD1306_set_cursor(uint8_t X, uint8_t Y)
{
    SSD1306_write_cmd(0x00 + (X & 0x0F)); // set column lower addres
    SSD1306_write_cmd(0x10 + ((X >> 4) & 0x0F)); // set column higher addres
    SSD1306_write_cmd(0xB0 + Y); // set page addres
}

void SSD1306_clear_display()
{
    for (uint8_t page = 0; page < 8; page++) {
        SSD1306_set_cursor(0, page);
        for (uint8_t column = 0; column < 128; column++) {
            SSD1306_write_data(0x00);
        }
    }

    SSD1306_set_cursor(0, 0);
}

uint SSD1306_get_font_index(char ch)
{
    return (uint)((ch-0x20)*0x08);
}

void SSD1306_write_char(char ch)
{
    uint char_index = SSD1306_get_font_index(ch);
    for (uint8_t i = 0; i < 8; i++) {
        SSD1306_write_data(*(font+char_index+i));
        printf("%d \n", char_index+i);
    }
}

void SSD1306_write_text(char *str)
{
    for (uint i = 0; i < strlen(str); i++) {
        SSD1306_write_char(toupper(str[i]));
    }
}

void SSD1306_write_image(struct SSD1306_image* image, uint8_t x, uint8_t y)
{
    uint8_t img_parts = image->height/8;
    for (uint i = 0; i < img_parts; i++) {
        SSD1306_set_cursor(x, i+y);
        for (uint j = 0; j < image->width; j++) {
            SSD1306_write_data(image->image[(i*image->width)+j]);
        }
    }
}

void SSD1306_init()
{
    SSD1306_write_cmd(SSD1306_SET_DISPLAY_ON);
    SSD1306_write_cmd(SSD1306_SET_DISPLAY_NORMAL);
    SSD1306_write_cmd(SSD1306_SET_MEMORY_MODE);
    SSD1306_write_cmd(SSD1306_PAGE_ADDRESSING_MODE);
    SSD1306_write_cmd(0x8D); //charge pump
    SSD1306_write_cmd(0x14);
    SSD1306_clear_display();
}

int main()
{
    stdio_init_all();
    sleep_ms(3000);
    printf("Initalizing \n");
    // I2C is "open drain", pull ups to keep signal high when no data is being
    // sent
    i2c_init(i2c_default, SSD1306_I2C_CLK * 1000);
    gpio_set_function(PICO_DEFAULT_I2C_SDA_PIN, GPIO_FUNC_I2C);
    gpio_set_function(PICO_DEFAULT_I2C_SCL_PIN, GPIO_FUNC_I2C);
    gpio_pull_up(PICO_DEFAULT_I2C_SDA_PIN);
    gpio_pull_up(PICO_DEFAULT_I2C_SCL_PIN);

    SSD1306_init();

    // SSD1306_write_text("#OPRSTUW");
    SSD1306_set_cursor(0, 0);
    SSD1306_write_text("SSD1306 DRIVER");
    SSD1306_set_cursor(0, 1);
    SSD1306_write_text("RPI");
    SSD1306_set_cursor(0, 2);
    SSD1306_write_text("Mateusz Zolisz 2023");

    // uint img_parts;

    // struct SSD1306_image* image = &image_raspberry;
    // struct SSD1306_image* image2 = &image_heart;

    SSD1306_write_image(&image_raspberry, 0, 4);
    SSD1306_write_image(&image_raspberry, image_raspberry.width + 12, 4);
    SSD1306_write_image(&image_raspberry, image_raspberry.width*2 + 24, 4);
    // for (int x = 0; 1; x+=5) {
    //     img_parts = image->height/8;
    //     for (uint i = 0; i < img_parts; i++) {
    //         SSD1306_set_cursor(x, i);
    //         for (uint j = 0; j < image->width; j++) {
    //             SSD1306_write_data(image->image[(i*image->width)+j]);
    //         }
    //     }

    //     img_parts = image2->height/8;
    //     for (uint i = 0; i < img_parts; i++) {
    //         SSD1306_set_cursor(x, i+4);
    //         for (uint j = 0; j < image2->width; j++) {
    //             SSD1306_write_data(image2->image[(i*image2->width)+j]);
    //         }
    //     }

    //     sleep_ms(200);

    //     for (uint i = 0; i < img_parts; i++) {
    //         SSD1306_set_cursor(x, i+4);
    //         for (uint j = 0; j < image2->width; j++) {
    //             SSD1306_write_data(0x00);
    //         }
    //     }

    //     for (uint i = 0; i < img_parts; i++) {
    //         SSD1306_set_cursor(x, i);
    //         for (uint j = 0; j < image->width; j++) {
    //             SSD1306_write_data(0x00);
    //         }
    //     }
    // }    

    for (;;) {
        //
    }   

    return 0;
}