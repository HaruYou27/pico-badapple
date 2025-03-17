"""Copyright 2025 - HaruYou27

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the “Software”), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE."""

from PIL import Image
import os
import numpy
import math

file = open("badapple.h", "w")

paths = []
for path in os.listdir():
    if path.endswith(".png"):
        paths.append(path)
paths.sort()

first_frame = True
page = 0
for path in paths:
    print(path)
    image = Image.open(path).convert('1')
    width = image.width
    height = image.height
    if first_frame:
        first_frame = False
        page = math.ceil(height / 8)
        frame = len(paths)
        file.write("static constexpr unsigned char BADAPPLE_WIDTH = " + str(width) + ';\n')
        file.write("static constexpr unsigned char BADAPPLE_PAGE = " + str(page) + ';\n')
        file.write("static constexpr int BADAPPLE_FRAME = " + str(frame) + ';\n')
        file.write("static constexpr unsigned char BADAPPLE_DATA[{frames}][{size}] =".format(frames=frame, size=image.width * page))
        file.write("{\n")
    img_array = numpy.array(image, dtype=numpy.uint8)
    
    c_array_str = "    {"
    for y in range(0, height, 8):
        for x in range(width):
            byte = 0
            for i in range(8):
                if y + i < height:
                    pixel = img_array[y + i, x]
                    # I have no idea how this works.
                    byte = (byte >> 1) | (0x80 if pixel == 0 else 0) 
            c_array_str += '0x%.2x,' % (byte)
    c_array_str = c_array_str + "},\n"
    file.write(c_array_str)
file.write("\n};")
file.close()