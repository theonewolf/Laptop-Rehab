TARGET=lr-tiny-amd64.iso
BUILD_DIR=/tmp/core-mount/

tools:
	gcc probe-blockdev.c \
		-static \
		-lblkid \
		-luuid \
		-o probe \
		-Wall -Werror

default:
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
