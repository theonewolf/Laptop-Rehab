/******************************************************************************
* probe-blockdev.c                                                            *
*                                                                             *
* Utility to help us find the boot partition and possibly the main HDD.       *
*                                                                             *
*                                                                             *
*   Authors: Wolfgang Richter <wolf@cs.cmu.edu>                               *
*                                                                             *
*   Copyright 2014 Laptop Rehab Project                                       *
*                                                                             *
* Permission is hereby granted, free of charge, to any person obtaining a     *
* copy of this software and associated documentation files (the "Software"),  *
* to deal in the Software without restriction, including without limitation   *
* the rights to use, copy, modify, merge, publish, distribute, sublicense,    *
* and/or sell copies of the Software, and to permit persons to whom the       *
* Software is furnished to do so, subject to the following conditions:        *
*                                                                             *
*     The above copyright notice and this permission notice shall be included *
*     in all copies or substantial portions of the Software.                  *
*                                                                             *
*     THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS *
*     OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF              *
*     MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  *
*     IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY    *
*     CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,    *
*     TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE       *
*     SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.                  *
*                                                                             *
******************************************************************************/
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>


#include <blkid/blkid.h>



#define USAGE "%s <label to search for> [-q]\n" \
    "\t\t-v: verbose mode"
#define BUF_SIZE 1024
#define OPTIONS "v"

void printstr(bool verbose, char* str)
{
    if (verbose)
    {
        fprintf(stdout, "%s", str);
    }
}

int main(int argc, char* argv[])
{
    blkid_cache cache;
    blkid_dev dev;
    char buf[BUF_SIZE], *error, *label, *devname;
    int o;
    bool verbose;

    if (argc < 2)
    {
        snprintf(buf, BUF_SIZE, USAGE, argv[0]);
        error = buf;
        goto error;
    }

    while ((o = getopt(argc, argv, OPTIONS)) != -1)
    {
        switch(o)
        {
            case 'v':
                verbose = true;
                break;
            case '?':
                error = "unknown option encountered";
                goto error;
            default:
                return EXIT_FAILURE;
        }
    }

    printstr(verbose, "-- Block device probe by Wolfgang Richter (CMU Laptop "
                    "Rehab Project) --\n");

    label = argv[optind];

    if (blkid_get_cache(&cache, NULL))
    {
        error = "could not load default cache";
        goto error;
    }

    snprintf(buf, BUF_SIZE, "-- Searching for target LABEL=%s --\n", label);
    printstr(verbose, buf);

    if ((dev = blkid_find_dev_with_tag(cache, "LABEL", label)) == NULL)
    {
        error = "could not find device with given label";
        goto error;
    }

    if ((devname = (char*) blkid_dev_devname(dev)) == NULL)
    {
        error = "could not get device name";
        goto error;
    }

    snprintf(buf, 1024, "-- Found device: %s -- \n", devname);
    printstr(verbose, buf);

    printstr(true, devname);

    return EXIT_SUCCESS;

error:
    fprintf(stderr, "Error: %s\n", error);
    return EXIT_FAILURE;
}
