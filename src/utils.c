#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <limits.h>
#include "utils.h"

/* load_file - convert a filepath into a content string
 * @path: the filepath
 *
 * Contracts:
 *  - @path is non-null and a valid file path
 * Responsibilities:
 *  - Call free() on the returned string
 */
char *load_file(const char *path)
{
    FILE *file = fopen(path, "rb");
    if (file == NULL) {
        FATAL("Could not open %s: %s", path, strerror(errno));
    }

    int ret = fseek(file, 0, SEEK_END);
    ASSERT(ret != -1, "fseek: %s", strerror(errno));
    int file_length = ftell(file);
    ASSERT(file_length != -1, "ftell: %s", strerror(errno));
    rewind(file);
    char *data = calloc(file_length + 1, sizeof (char));
    ASSERT(data != NULL, "Out of memory");
    ret = fread(data, sizeof (char), file_length, file);
    ASSERT(ret == file_length, "fread");
    ret = fclose(file);
    ASSERT(ret != EOF, "fclose: %s", strerror(errno));
    return data;
}

/* my_getline() - reads a line from @stream into @lineptr
 * @lineptr: where to place the pointer to the buffer
 * @n: where to place the size of the buffer
 * @stream: the FILE to read from
 *
 * Returns: the number of characters read, not including '\0'
 *
 * Set *lineptr to NULL and *n to 0 before calling my_getline for the first time
 * Call free(*lineptr) for the last call to my_getline
 */
long my_getline(char **lineptr, size_t *n, FILE *stream)
{
    return my_getdelim(lineptr, n, '\n', stream);
}

long my_getdelim(char **lineptr, size_t *n, int delim, FILE *stream)
{
    /* The first call to the function */
	if (*lineptr == NULL || *n == 0) {
		*n = BUFSIZ;
		if ((*lineptr = malloc(*n)) == NULL)
			return -1;
	}

	char *ptr = *lineptr, 
         *eptr = *lineptr + *n;

	for (;;) {
		int c = fgetc(stream);

		if (c == EOF) {
			if (feof(stream)) {
				long diff = ptr - *lineptr;
				if (diff != 0) {
					*ptr = '\0';
					return diff;
				}
			}
			return -1;
		}

		*ptr++ = c;
		if (c == delim) {
			*ptr = '\0';
			return ptr - *lineptr;
		}
		if (ptr + 2 >= eptr) {
			char *nbuf;
			size_t nbufsiz = *n * 2;
			long d = ptr - *lineptr;
			if ((nbuf = realloc(*lineptr, nbufsiz)) == NULL)
				return -1;
			*lineptr = nbuf;
			*n = nbufsiz;
			eptr = nbuf + nbufsiz;
			ptr = nbuf + d;
		}
	}
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
