# Current Findings

As of Ubuntu 14.04 LTS they no longer support the alternate installer.  They
only support Ubiquity it seems, which means we can not fully preseed things.

An alternate approach is creating an OEM install of Ubuntu onto a virtual disk
then using a small boot image to copy that image onto the disk of a target
system.

The small boot image could be built based off of minimal distribution of Linux
such as Tinycore.

## Tinycore Path

This seems the most viable.  Current work is promising with support from the
`blkid` tool.

Currently using `xz` to compress system images, but it is very slow
decompressing (or KVM slows xz down).  Current USB size is ~1.1 Gibibytes.
This is acceptable, and only marginally larger than the Ubuntu install ISO!

Idea for Ubuntu installation:

```
    blkid -L "CMU Laptop Rehab 64-bit" # provides partition USB fs
    blkid -L "CMU Laptop Rehab 64-bit" | grep sd[^0-9]* -o # provides device
```

after mounting the `blkid` result, we unpack the compressed Ubuntu disk image
to `/dev/hda` if the USB fs was `/dev/sda`.  Otherwise we unpack to `/dev/sda`.

The idea here is that we want to unpack onto the first hard drive in the
system.

Then we resize the root partition via fdisk to takeover the entire disk.

### TinyCore Customizations

TinyCore doesn't come with a lot of tools that we'd want, but we can easily
extend it during the remastering step.  While remastering, we should mix-in:

```
    e2fsprogs.tcz 
    ncurses-common.tcz 
    ncurses.tcz 
    udev-lib.tcz 
    util-linux.tcz
```

Which will give us tools to use for the rest of the installation procedure.

These extensions can be mixed in by using `unsquashfs` to unpack them, and then
`rsync` them into the TinyCore root file system.

### Finding First HDD

We assume there is only one main HDD in the system that the user will want to
install an OS onto.  If we have `lsblk` available for us in the TinyCore world,
we can search for such a device with just a bit of bash:

```
    lsblk -o NAME -e 1,2,11 -b -n -d | grep -v sdb | head -n 1
```

Here we exclude memory, fd, and ROM devices.  We might want to add more to our
exclusion list in the future.  We also exclude the USB device (sdb in the
example) via `grep`.  This can be programmatically found via the aforementioned
`blkid` command.

### Minimizing Ubuntu Install Disk Size

There are several usable techniques here.  First, we should zero all unused 
blocks to improve compression with a tool like `fstrim`.  Second, we should
deduplicate all the blocks (but this needs a new tool to be developed for both
packing and unpacking).  Third, we need very good compression, with very fast
decompression.

### Resizing Partition

There is only one partition we care about resizing, and that is the root
partition.  We pre-setup the image so that it is the last partition and
therefore easy to expand to the end of whatever drive we end up on.

This `sfdisk` command line should only modify the last partition:

```
    sfdisk -N4 /dev/sda << EOF
        ,+
        EOF
```

We only really need to change the partition number.  The default for `sfdisk`
is to keep old values, so we use `+` for the size to use maximum size.

After running that, we then need to use `resize2fs` to make the file system
actually use that space:

```
    resize2fs /dev/sda4
```

### Bootable Script for Tinycore

Currently executing from `init -> rcS -> tc-config -> bootsync.sh`.

Unresolved issue: Why isn't the `fstab` file properly created in time?

### Using `pv`

It's unsettling watching the unpacking with 0 status or feedback.  To remedy
this I've produced a static build of `pv` and included it in our TinyCore
install.

The command we are most likely to use is:

```
    gunzip /mnt/sdb1/Ubuntu32.img.gz | pv -s 6.5g -p -t -eta -r -B 512k > /dev/sda
```
