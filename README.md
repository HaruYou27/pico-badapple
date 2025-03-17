# Why another?
I know that someone done this on the pico.

But this one run at 30fps without any cut content.

# Hardware requirement
You will need a variant of the pico with 16MB of flash to use this.

SSD1306 I2C OLED 128x64

# Note
I use pin 16 for SDA and pin 17 for SCL, change the parameters in `main.cpp` to your setup.

You may have to change `PICO_BOARD` in `CMakeLists.txt` file to your board specific.

I use `weact_studio_rp2040_16mb` board.

The `uf2` file I built is 8.7MB. However the `elf` file is only 4.7MB.

It might be possible to optimize and cram this into 8MB flash board.

# How 2 build
1. Install Pico C SDK to your PATH, or in this directory.
2. Create build directory `mkdir build && cd build`
3. `cmake ..`
4. If `cmake` run without issue, you can proceed to build `make -j 8`
5. After `make` finish build, flash the `.uf2` file to your pico.

# Link
https://youtu.be/89mWeVJCcXk
