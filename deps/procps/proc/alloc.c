/*
 * alloc.c - memory allocation functions
 * Copyright (C) 1992-1998 by Michael K. Johnson, johnsonm@redhat.com
 * Copyright 2002 Albert Cahalan
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 */

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "alloc.h"

static void xdefault_error(const char *restrict fmts, ...) __attribute__((format(printf,1,2)));
static void xdefault_error(const char *restrict fmts, ...) {
    va_list va;

    va_start(va, fmts);
    fprintf(stderr, fmts, va);
    va_end(va);
}

message_fn xalloc_err_handler = xdefault_error;


void *xcalloc(unsigned int size) {
    void * p;

    if (size == 0)
        ++size;
    p = calloc(1, size);
    if (!p) {
        xalloc_err_handler("%s failed to allocate %u bytes of memory", __func__, size);
        exit(EXIT_FAILURE);
    }
    return p;
}

void *xmalloc(unsigned int size) {
    void *p;

    if (size == 0)
        ++size;
    p = malloc(size);
    if (!p) {
        xalloc_err_handler("%s failed to allocate %u bytes of memory", __func__, size);
        exit(EXIT_FAILURE);
    }
    return(p);
}

void *xrealloc(void *oldp, unsigned int size) {
    void *p;

    if (size == 0)
        ++size;
    p = realloc(oldp, size);
    if (!p) {
        xalloc_err_handler("%s failed to allocate %u bytes of memory", __func__, size);
        exit(EXIT_FAILURE);
    }
    return(p);
}

char *xstrdup(const char *str) {
    char *p = NULL;

    if (str) {
        unsigned int size = strlen(str) + 1;
        p = malloc(size);
        if (!p) {
            xalloc_err_handler("%s failed to allocate %u bytes of memory", __func__, size);
            exit(EXIT_FAILURE);
        }
        strcpy(p, str);
    }
    return(p);
}
