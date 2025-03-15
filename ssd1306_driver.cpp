/*Copyright 2025 - HaruYou27

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the “Software”), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.*/

#include "ssd1306_driver.hpp"

SSD1306::~SSD1306() {}

using namespace std;

int SSD1306::clear_screen()
{
	set_write_position(0, 0);
	
	static constexpr uint8_t buffer[MAX_BUFFER_SIZE] = {CONTROL_DATA};
	return i2c_write(buffer, MAX_BUFFER_SIZE);	
}

int SSD1306::draw_image_fullscreen(const uint8_t *data, int width, int pages, bool invert)
{
	uint8_t *buffer = new uint8_t[MAX_BUFFER_SIZE] {CONTROL_DATA};
	int offset = (SCREEN_WIDTH - width) / 2;

	if (!offset)
	{
		size_t size = width * pages;
		if (invert)
		{
			for (int index = 0; index < size; index++)
			{
				buffer[index+1] = 0xff - data[index];
			}
		}
		else
		{
			memcpy(buffer + 1, data, size);
		}
		int error = i2c_write(buffer, MAX_BUFFER_SIZE);
		delete[] buffer;
		return error;
	}

	uint8_t *begin = buffer;
	offset += 1;
	for (int page = 1; page <= pages; page++)
	{
		if (invert)
		{
			uint8_t *end = buffer + SCREEN_WIDTH;
			buffer += offset;
			for (int index = 0; index < width; index++)
			{
				*buffer = 0xff - *data;
			}
			data += width;
			buffer = end;
			continue;
		}
		memcpy(buffer + offset, data, width);
		data += width;
		buffer += SCREEN_WIDTH;
	}
	int error = i2c_write(begin, MAX_BUFFER_SIZE);
	delete[] begin;
	return error;
}

int SSD1306::print_text(const char *text, size_t size, bool invert)
{
	uint8_t *buffer = new uint8_t[size] {CONTROL_DATA};

	int index = 1;
	int index_page = 1;
	while (*text && index < size)
	{
		if (*text == '\n')
		{
			index_page += SCREEN_WIDTH;
			if (invert)
			{
				fill(buffer + index, buffer + index_page, 0xff);
			}
			index = index_page;
			++text;
			continue;
		}

		int character = *text - 32;
		++text;
		if (character < 0)
		{
			continue;
		}

		if (invert)
		{
			for (int i = 0; i < MoonBench4x8_WIDTH; i++)
			{
				buffer[index++] = 0xff - MoonBench4x8[character][i];
			}
			continue;
		}
		memcpy(buffer + index, MoonBench4x8[character], MoonBench4x8_WIDTH);
		index += MoonBench4x8_WIDTH;
	}

	int error = i2c_write(buffer, size);
	delete[] buffer;
	return error;
}

int SSD1306::set_char_pos(uint8_t column, uint8_t line)
{
	uint8_t x = column * MoonBench4x8_WIDTH;
	uint8_t y = line * PAGE_SIZE;

	if (x >= SCREEN_WIDTH)
	{
		x = 0;
	}
	if (y >= SCREEN_HEIGHT)
	{
		y = 0;
	}
	return set_write_position(x, y);
}

int SSD1306::set_contrast(uint8_t value)
{
	uint8_t buffer[3] = 
	{
		CONTROL_COMMAND,
		SET_CONTRAST,
		value,
	};
	return i2c_write(buffer, 3);
}

int SSD1306::set_write_position(uint8_t x, uint8_t y)
{
	uint8_t buffer[4] = 
	{
		CONTROL_COMMAND,
		0x10 | static_cast<uint8_t>(x >> 4), // higher column address
		x & 0x0F, // lower column address
		0xB0 + (y / PAGE_SIZE), // row address
	};
	return i2c_write(buffer, 4);
}

int SSD1306::i2c_write(const uint8_t* buffer, int length)
{
	return i2c_write_blocking(i2c, SLAVE_ADDRESS, buffer, length, false);
}

// Print and overwrite a single line.
int SSD1306::print_line(const char *text, uint8_t line, bool invert)
{
	set_char_pos(0, line);
	return print_text(text, SCREEN_WIDTH + 1, invert);
}

// Clear the screen then print text.
int SSD1306::print_overwrite(const char *text, bool invert)
{
	set_write_position(0, 0);
	return print_text(text, MAX_BUFFER_SIZE, invert);
}

SSD1306::SSD1306(uint sda, uint scl, i2c_inst_t *instance_i2c)
{
    i2c = instance_i2c;
    i2c_init(i2c, BAUDRATE);
	gpio_set_function(sda, GPIO_FUNC_I2C);
	gpio_set_function(scl, GPIO_FUNC_I2C);
	gpio_pull_up(sda);
	gpio_pull_up(scl);

	// init sequence based on https://github.com/adafruit/Adafruit_SSD1306
	static constexpr uint8_t buffer[30] = {
		CONTROL_COMMAND,
		DISPLAY_OFF,
		SET_DISPLAY_CLOCK_DIV, 0x80,
		SET_MULTIPLEX, 0x3F,
		SET_DISPLAY_OFFSET, 0,
		SETSTARTLINE | 0,
		CHARGEPUMP, 0x14,
		SEGREMAP | 0x01,
		COMSCANDEC,
		SET_COM_PINS, 0x12,
		SET_CONTRAST, 0x7F,
		SET_PRECHARGE, 0xF1,
		SETVCOMDETECT, 0x40,
		DISPLAY_ALL_ON_RESUME,
		NORMAL_DISPLAY,
		SET_MEMORY_MODE,
		0, // Horizontal addressing mode
		2, // column address = 2
		0x10,
		0x40, // display start line = 0
		0xB0, // page start = 0
		DISPLAY_ON,
	};
	i2c_write(buffer, 30);
}