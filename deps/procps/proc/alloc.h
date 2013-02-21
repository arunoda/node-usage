#ifndef PROCPS_PROC_ALLOC_H
#define PROCPS_PROC_ALLOC_H

#include "procps.h"

EXTERN_C_BEGIN

 /* change xalloc_err_handler to override the default fprintf(stderr... */
extern message_fn xalloc_err_handler;

extern void *xcalloc(unsigned int size) MALLOC;
extern void *xmalloc(unsigned int size) MALLOC;
extern void *xrealloc(void *oldp, unsigned int size) MALLOC;
extern char *xstrdup(const char *str) MALLOC;

EXTERN_C_END

#endif
