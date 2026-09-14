#include "../inc/util.h"

#include <stdio.h>
#include <unistd.h>

int chk_res_exists(const char *path)
{
        if (!path) {
                fprintf(stderr, "Error: Resource path not provided\n");
                return -2;
        }

        return access(path, F_OK) == 0 ? 0 : -1;
}

int load_config(const char *path)
{
        if (!path) {
                fprintf(stderr, "Error: Resource path not provided\n");
                return -1;
        }

        // FIXME: Add the implementation of loading the configuration from the
        // specified INI file.

        return 0;
}
