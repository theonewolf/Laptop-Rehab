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

    return EXIT_SUCCESS;

error:
    fprintf(stderr, "Error: %s\n", error);
    return EXIT_FAILURE;
}
