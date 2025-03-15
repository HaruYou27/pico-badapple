/*Copyright 2025 - HaruYou27

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the “Software”), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.*/


#include "ssd1306_driver.hpp"
#include "bad_apple.h"

int main()
{
    gpio_init(25);
    gpio_set_dir(25, GPIO_OUT);
    gpio_put(25, true);

    SSD1306 *oled = new SSD1306(16, 17, i2c0);

    bool tick = true;
    while (true)
    {
        oled->print_overwrite("This is a fanwork of\nTouhou Project by ZUN.\nBad Apple!!-Alstroemeria Records\nRaspberry Pi pico 16MB flash\nSSD1306 I2C OLED 128x64\nFont by Moonbench.xyz\nHaruYou27");
        sleep_ms(5000);
        for (int index = 0; index < BAD_APPLE_FRAME; index++)
        {
            tick = !tick;
            oled->draw_image_fullscreen(BAD_APPLE_DATA[index], BAD_APPLE_WIDTH, BAD_APPLE_PAGE, tick);
        }
    }
}