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
    blkid -L "CMU Laptop Rehab 64-bit" # provides mountable location of USB fs
```

after mounting the `blkid` result, we unpack the compressed Ubuntu disk image
to `/dev/hda` if the USB fs was `/dev/sda`.  Otherwise we unpack to `/dev/sda`.

The idea here is that we want to unpack onto the first hard drive in the
system.

Then we resize the root partition via fdisk to takeover the entire disk.
