"""Copyright 2025 - HaruYou27

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the “Software”), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE."""

from PIL import Image
import os
import numpy
import math

first_frame = True

NAME = "BAD_APPLE"
files = []

file = open("bad_apple.h", "w")
for path in os.listdir():
    if path.endswith(".png"):
        files.append(path)
files.sort()

for path in files:
    print(path)
    image = Image.open(path)
    width = image.width
    height = image.height
    page = math.ceil(height / 8)
    if first_frame:
        first_frame = False
        file.write("static constexpr int {name}_FRAME = {size};\n".format(name=NAME, size=len(files)))
        file.write("static constexpr unsigned char {name}_WIDTH = {w};\nstatic constexpr unsigned char {name}_PAGE = {h};\nstatic constexpr unsigned char {name}_DATA[{frame}][{w}] = ".format(name=NAME, w=width, h=page, frame=len(files)))
        file.write("{\n")
    img_array = numpy.array(image, dtype=numpy.uint8)
    c_array = []

    # Iterate over the image in 8-pixel vertical chunks
    for y in range(0, page, 8):
        for x in range(width):
            # Extract 8 vertical pixels
            byte = 0
            for i in range(8):
                if y + i < height:
                    pixel = img_array[y + i, x]
                    byte |= (pixel & 0x01) << (7 - i)
            c_array.append(byte)
    c_array_str = "{"
    for i, byte in enumerate(c_array):
        c_array_str += f"0x{byte:02X}, "
    c_array_str = c_array_str + "},\n"
    file.write(c_array_str)
file.write("};")
file.close()