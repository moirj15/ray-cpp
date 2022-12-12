#include <stdlib.h>
#include <stdio.h>

#include "utils.h"

void quit(const char *qMessage) {
    fprintf(stderr, "%s\n", qMessage);
    exit(EXIT_FAILURE);
}

/**
 * Implementation of the getline function provided in GCC.
 *
 * @param lineptr: Pointer to a string, will be re-allocated as needed.
 * @param n: The size of the string buffer.
 * @param stream: The file that will be read from.
 * @return: Number of characters read on success, otherwise -1.
 */
s32 getline(char **lineptr, u64 *n, FILE *stream)
{
    static char line[256];
    char *ptr;
    unsigned int len;

    if (lineptr == NULL || n == NULL)
    {
        errno = EINVAL;
        return -1;
    }

    if (ferror(stream))
        return -1;

    if (feof(stream))
        return -1;

    fgets(line, 256, stream);

    ptr = strchr(line, '\n');
    if (ptr)
        *ptr = '\0';

    len = strlen(line);

    if ((len + 1) < 256)
    {
        ptr = (char *)realloc(*lineptr, 256);
        if (ptr == NULL)
            return(-1);
        *lineptr = ptr;
        *n = 256;
    }

    strcpy(*lineptr, line);
    return(len);
}









