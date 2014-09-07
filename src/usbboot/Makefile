TARGET=lr-amd64.iso
BUILD_DIR=/tmp/lr64-distribution/

default:
	rsync -avxz lr64-distribution/ /tmp/lr64-distribution/
	sudo mkisofs -r -V "Laptop Rehab Ubuntu 14.04 64-bit Install" \
		-cache-inodes \
		-J -l -b isolinux/isolinux.bin \
		-c isolinux/boot.cat -no-emul-boot \
		-boot-load-size 4 -boot-info-table \
		-o ${TARGET} ${BUILD_DIR}
	isohybrid ${TARGET}

test:
	kvm \
		-usbdevice disk:${TARGET} \
		-boot menu=on \
		-m 2G
