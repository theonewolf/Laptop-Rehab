###############################################################################
# Makefile                                                                    #
#                                                                             #
# Makefile for the USB/CD image, testing it, and associated utilities.        #
#                                                                             #
#                                                                             #
#   Authors: Wolfgang Richter <wolf@cs.cmu.edu>                               #
#                                                                             #
#   Copyright 2014 Laptop Rehab Project                                       #
#                                                                             #
# Permission is hereby granted, free of charge, to any person obtaining a     #
# copy of this software and associated documentation files (the "Software"),  #
# to deal in the Software without restriction, including without limitation   #
# the rights to use, copy, modify, merge, publish, distribute, sublicense,    #
# and/or sell copies of the Software, and to permit persons to whom the       #
# Software is furnished to do so, subject to the following conditions:        #
#                                                                             #
#     The above copyright notice and this permission notice shall be included #
#     in all copies or substantial portions of the Software.                  #
#                                                                             #
#     THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS #
#     OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF              #
#     MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  #
#     IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY    #
#     CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,    #
#     TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE       #
#     SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.                  #
#                                                                             #
###############################################################################
TARGET=lr-tiny-amd64.iso
BUILD_DIR=/tmp/core-mount/

tools:
	gcc probe-blockdev.c \
		-static \
		-lblkid \
		-luuid \
		-o probe \
		-Wall -Werror

core.gz:
	cd core-unpacked && \
		sudo find | sudo cpio -o -H newc | gzip -2 > ../core.gz && \
		cd .. && \
		advdef -z4 core.gz && \
		sudo mv core.gz core-mount/boot/

lr-tiny-amd64.iso: core.gz
	rsync -avxz core-mount/ /tmp/core-mount/
	sudo mkisofs -r -V "CMU Laptop Rehab 64-bit" \
		-cache-inodes \
		-J -l -b boot/isolinux/isolinux.bin \
		-c boot/isolinux/boot.cat -no-emul-boot \
		-boot-load-size 4 -boot-info-table \
		-o ${TARGET} ${BUILD_DIR}
	sudo isohybrid ${TARGET}

test: default
	truncate hdd.raw --size 8G
	kvm \
		-usbdevice disk:${TARGET} \
		-drive file=hdd.raw \
		-boot menu=on \
		-m 2G \
		-cpu host
