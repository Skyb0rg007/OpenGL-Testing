#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <limits.h>
#include "utils.h"

char *load_file(const char *path)
{
    FILE *file = fopen(path, "rb");
    if (file == NULL) {
        FATAL("Could not open %s: %s", path, strerror(errno));
    }

    fseek(file, 0, SEEK_END);
    int file_length = ftell(file);
    rewind(file);
    char *data = calloc(file_length + 1, sizeof (char));
    fread(data, sizeof (char), file_length, file);
    fclose(file);
    return data;
}


int my_getline(char **lineptr, size_t *n, FILE *stream)
{
    static char line[256];
    char *ptr;
    unsigned int len;

    if (lineptr == NULL || n == NULL)
    {
        errno = EINVAL;
        return -1;
    }

    if (ferror (stream))
        return -1;

    if (feof(stream))
        return -1;

    fgets(line,256,stream);

    ptr = strchr(line,'\n');   
    if (ptr)
        *ptr = '\0';

    len = strlen(line);

    if ((len+1) < 256)
    {
        ptr = realloc(*lineptr, 256);
        if (ptr == NULL)
            return(-1);
        *lineptr = ptr;
        *n = 256;
    }

    strcpy(*lineptr,line); 
    return len;
}

float tofloat(const char *str)
{
    ASSERT(str != NULL, "Passed null to tofloat()");
    char *endptr;
    errno = 0;
    float n = strtof(str, &endptr);
    if (endptr == str || errno) {
        FATAL("Error turning %s into a float!", str);
    }
    return n;
}

unsigned touint(const char *str)
{
    ASSERT(str != NULL, "Passed null to toint()");
    char *endptr;
    errno = 0;
    unsigned long n = strtoul(str, &endptr, 10);
    if (str == endptr || errno) {
        FATAL("Error turning %s into an unsigned int!", str);
    }
    if (n > UINT_MAX) {
        FATAL("%s is too big to fit in an unsigned int!", str);
    }
    return n;
}
