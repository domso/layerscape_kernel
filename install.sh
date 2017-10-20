#!/bin/bash

echo "Start Installation"

COMPILER_INSTALL_DIR=/usr/

echo "Download Compiler"
wget https://releases.linaro.org/components/toolchain/binaries/latest-5/aarch64-elf/gcc-linaro-5.4.1-2017.05-x86_64_aarch64-elf.tar.xz

echo "Install Compiler"
sudo tar xf gcc-linaro-5.4.1-2017.05-x86_64_aarch64-elf.tar.xz -C $COMPILER_INSTALL_DIR --strip 1

rm gcc-linaro-5.4.1-2017.05-x86_64_aarch64-elf.tar.xz

echo "Install tftp"
sudo apt-get install xinetd tftpd tftp u-boot-tools cmake screen
sudo mkdir /var/tftproot
sudo chown -R nobody /var/tftproot
sudo chmod -R 777 /var/tftproot

sudo sh -c "echo 'service tftp
	{
	  protocol        = udp
	  port            = 69
	  socket_type     = dgram
	  wait            = yes
	  user            = nobody
	  server          = /usr/sbin/in.tftpd
	  server_args     = /var/tftproot
	  disable         = no
    }' > /etc/xinetd.d/tftp"

sudo service xinetd restart

./configure.sh
