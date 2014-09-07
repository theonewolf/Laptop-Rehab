TARGET=lr-amd64.iso
BUILD_DIR=/tmp/lr64-distribution/

default:
	rsync -avxz lr64-distribution/ /tmp/lr64-distribution/
	sudo mkisofs -r -V "CMU Laptop Rehab 64-bit" \
		-cache-inodes \
		-J -l -b isolinux/isolinux.bin \
		-c isolinux/boot.cat -no-emul-boot \
		-boot-load-size 4 -boot-info-table \
		-o ${TARGET} ${BUILD_DIR}
	sudo isohybrid ${TARGET}

test:
	truncate hdd.raw --size 8G
	kvm \
		-usbdevice disk:${TARGET} \
		-drive file=hdd.raw \
		-boot menu=on \
		-m 2G
