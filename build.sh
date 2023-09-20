#/bin/bash

c++ $(pkg-config Magick++ -cflags -libs) -O3 main.cc -o betterButtonMaker.elf
