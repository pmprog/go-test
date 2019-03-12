#!/bin/bash

echo "Removing previous build"
rm build/odroid-go-common.bin
rm marqtest.fw

echo "Compiling Source"
make -j4 all

if [ ! -f tile.raw ]; then
	echo "Making image"
	ffmpeg -i tile.png -f rawvideo -pix_fmt rgb565 tile.raw
fi

if [ -f build/odroid-go-common.bin ]; then
	echo "Compiling .fw"
	../odroid-go-firmware/tools/mkfw/mkfw "MarqTest" tile.raw 0 16 1048576 test build/odroid-go-common.bin
	mv firmware.fw marqtest.fw

	echo "Compilation Complete"
fi