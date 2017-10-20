#!/bin/bash

for dir in $(find -maxdepth 1 -mindepth 1 -type d)
do
	echo "Configure $dir"
	cd $dir

	ln -s /var/tftproot tftp

	mkdir build	

	cd build
	cmake -DCMAKE_TOOLCHAIN_FILE=../toolchain.cmake ..
	cmake -DCMAKE_TOOLCHAIN_FILE=../toolchain.cmake ..	#dont ask why... °_°
	make

	cd ../..
done
