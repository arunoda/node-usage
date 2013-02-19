#include <stdlib.h>
#include <unistd.h>
#include "sysinfo.h"
#include "readproc.h"

typedef proc_t usage_t;

extern usage_t* usage_init(int);
/*
	CPU Usage as a percentage
*/
extern float usage_cpu(usage_t*);
/*
	Total memory usage in killobytes
*/
extern unsigned long usage_totalmem(usage_t*);
extern void usage_clean(usage_t*);
