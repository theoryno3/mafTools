#include <assert.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include "common.h"

void* de_malloc(size_t n) {
    void *i;
    i = malloc(n);
    if (i == NULL) {
        fprintf(stderr, "malloc failed on a request for %zu bytes\n", n);
        exit(EXIT_FAILURE);
    }
    return i;
}

int32_t de_getline(char **s, int32_t *n, FILE *f) {
    register int32_t nMinus1 = ((*n) - 1), i = 0;
    char *s2 = *s;
    while (1) {
        register int32_t ch = (char) getc(f);
        if (ch == '\r')
            ch = getc(f);
        if (i == nMinus1) {
            *n = 2 * (*n) + 1;
            *s = realloc(*s, (*n + 1));
            assert(*s != NULL);
            s2 = *s + i;
            nMinus1 = ((*n) - 1);
        }
        if ((ch == '\n') || (ch == EOF)) {
            *s2 = '\0';
            return(feof(f) ? -1 : i);
        } else {
            *s2 = ch;
            s2++;
        }
        ++i;
    }
}

void verbose(char const *fmt, ...) {
    extern int verbose_flag;
    char str[d_MAX_MESSAGE_LENGTH];
    va_list args;
    va_start(args, fmt);
    if (verbose_flag) {
        int n = vsprintf(str, fmt, args);
        if (n >= d_MAX_MESSAGE_LENGTH) {
            fprintf(stderr, "Error, failure in verbose(), (n = %d) > "
                    "(d_MAX_MESSAGE_LENGTH %d)\n", n, d_MAX_MESSAGE_LENGTH);
            exit(EXIT_FAILURE);
        }
        message("Verbose", str, args);
    }
    va_end(args);
}

void debug(char const *fmt, ...) {
    extern int debug_flag;
    char str[d_MAX_MESSAGE_LENGTH];
    va_list args;
    va_start(args, fmt);
    if (debug_flag) {
        int n = vsprintf(str, fmt, args);
        if (n >= d_MAX_MESSAGE_LENGTH) {
            fprintf(stderr, "Error, failure in debug(), (n = %d) > "
                    "(d_MAX_MESSAGE_LENGTH %d)\n", n, d_MAX_MESSAGE_LENGTH);
            exit(EXIT_FAILURE);
        }
        message("Debug", str, args);
    }
    va_end(args);
}

void message(char const *type, char const *fmt, ...) {
    va_list args;
    va_start(args, fmt);
    fprintf(stderr, "%s: ", type);
    vfprintf(stderr, fmt, args);
    va_end(args);
}