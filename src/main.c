#include "SSD1306_I2C.c"

/**
 * Example demo code for usage of SSD1306 I2C Driver.
 */

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

    SSD1306_SetFont(&dev, "5x8");
    SSD1306_WriteText(&dev, "RASPBERRY PI PICO");
    SSD1306_NextLine(&dev);
    SSD1306_SetFont(&dev, "8x16");
    SSD1306_WriteText(&dev, "SSD1306 I2C");
    SSD1306_NextLine(&dev);
    SSD1306_WriteText(&dev, "Driver");
    SSD1306_SetFont(&dev, "8x8");
    SSD1306_NextLine(&dev);
    SSD1306_WriteImage(&dev, &image_raspberry, 80, 4); 

    SSD1306_SetFont(&dev, "8x16");
    char str[3];
   
    SSD1306_SetCursor(&dev, 0, 5);
    
    for (int i = 10; i > 0; i--) {
        SSD1306_ClearSpace(&dev, 0, 16, 5, 6);
        SSD1306_SetCursor(&dev, 0, 5);
        sprintf(str, "%d", i);
        SSD1306_WriteText(&dev, str);
        sleep_ms(1000);

        if (i == 1) {
            i = 11;
        }
    }  

    return 0;
}