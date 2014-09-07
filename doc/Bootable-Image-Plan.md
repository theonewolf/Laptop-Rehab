# Current Findings

As of Ubuntu 14.04 LTS they no longer support the alternate installer.  They
only support Ubiquity it seems, which means we can not fully preseed things.

An alternate approach is creating an OEM install of Ubuntu onto a virtual disk
then using a small boot image to copy that image onto the disk of a target
system.

The small boot image could be built based off of minimal distribution of Linux
such as Tinycore.
