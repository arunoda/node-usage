#ifndef PROCPS_PROC_ESCAPE_H
#define PROCPS_PROC_ESCAPE_H

//#include <stdio.h>
#include <sys/types.h>
#include "procps.h"
#include "readproc.h"

EXTERN_C_BEGIN

#define ESC_STRETCH 1  // since we mangle to '?' this is 1 (would be 4 for octal escapes)

#define ESC_ARGS     0x1  // try to use cmdline instead of cmd
#define ESC_BRACKETS 0x2  // if using cmd, put '[' and ']' around it
#define ESC_DEFUNCT  0x4  // mark zombies with " <defunct>"

extern int escape_strlist(char *__restrict dst, char *__restrict const *__restrict src, size_t n, int *cells);
extern int escape_str(char *__restrict dst, const char *__restrict src, int bufsize, int *maxcells);
extern int escape_command(char *__restrict const outbuf, const proc_t *__restrict const pp, int bytes, int *cells, unsigned flags);
extern int escaped_copy(char *__restrict dst, const char *__restrict src, int bufsize, int *maxroom);

EXTERN_C_END
#endif
